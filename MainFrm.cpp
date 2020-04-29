//---------------------------------------------------------------------------

#include <vcl.h>
#include <math.hpp>
#include <IniFiles.hpp>
#pragma hdrstop

#include "MainFrm.h"
#include "ExportFrm.h"
#include "HexFrm.h"
#include "SelPatFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ieview"
#pragma link "imageenview"
#pragma link "imageenio"
#pragma resource "*.dfm"
TMainForm *MainForm;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
    : TForm(Owner)
{
    int i;

    BinData=NULL;
    BinDataLen=0;

    ByteData=NULL;    // byte output (used for HexForm and save .bin to disk)
    ByteDataLen=0;

    Endianness = 1;
    
    NumberOfBits=0;
    MouseDownSelecting=0;     // is mouse button down, selecting bits

    ReverseOutputs=0;
    ReverseWithinOutCol=0;
    ROM_Layout = ROM_STANDARD;

    for( i=0; i<MAX_ROM_OUTPUTS; i++ )
        OutputOrder[i] = i;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::Exit1Click(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::DrawBitsButtonClick(TObject *Sender)
{
    int OneColor = clBlack;     // set colours used
    int ZeroColor = clWhite;
    int GapColor = clSilver;
    int SelOneColor = clBlue;
    int SelZeroColor = clYellow;
    int bitcolor;


    SetupVariables();

// calculate total number of pixels in BitImage
    BitImageWidth = BitsPerRow * (BitSizeX+BitGap) + (ColGap * ( BitsPerRow / BitsPerOutput - 1 ) ) - BitGap;
    BitImageHeight = (NumberOfBits / BitsPerRow) * (BitSizeX+BitGap) - BitGap;

    BitImageWidth += 11;
    BitImageHeight += 11;
// allocate new BitImageEnView->IEBitmap
    BitImageEnView->IEBitmap->Allocate( BitImageWidth, BitImageHeight, ie24RGB);

// fill in entire BitImage with GapColor
    BitImageEnView->IEBitmap->FillRect( 0, 0, BitImageWidth, BitImageHeight, GapColor );


// loop through NumberOfBits, a byte at a time (BitCounter is incremented by inner BitInByteCounter loop)
    for( int BitCounter=0; BitCounter < NumberOfBits; BitCounter++ )
    {
// read input byte from bin file

            int BitInRow;
            int RowNumber;

            BitInRow = BitCounter % BitsPerRow;
            RowNumber = BitCounter / BitsPerRow;

            if( BinData[BitCounter] & ROM_BIT )         // is 1 bit
            {
                if( BinData[BitCounter] & ROM_SEL )
                    bitcolor = SelOneColor;
                else
                    bitcolor = OneColor;
            }
            else                                        // is 0 bit
            {
                if( BinData[BitCounter] & ROM_SEL )
                    bitcolor = SelZeroColor;
                else
                    bitcolor = ZeroColor;
            }

            BitImageEnView->IEBitmap->FillRect( 5 + BitInRow * (BitSizeX+BitGap) + BitInRow/BitsPerOutput * ColGap, 5 + RowNumber * (BitSizeY+BitGap), 5 + BitInRow * (BitSizeX+BitGap) + BitInRow/BitsPerOutput * ColGap + BitSizeX, 5 + RowNumber * (BitSizeY+BitGap) + BitSizeY, bitcolor);
    }

    BitImageEnView->Update();

    if( ExportBin() > 0 )   // ByteData[] has been changed
        HexForm->UpdateBin( ByteData, ByteDataLen );

    return;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::OpenBinFileButtonClick(TObject *Sender)
{
    AnsiString ext;

    if( WorkingDir.Length() )
    {
        FileOpenDialog->InitialDir=WorkingDir;
        FileOpenDialog->FileName="";
    }
    if(FileOpenDialog->Execute() != mrOk)
        return;
    WorkingDir = FileOpenDialog->FileName.SubString(1, FileOpenDialog->FileName.LastDelimiter( ":\\" ) );

    // check extension and call openbin if not .txt
    if( FileOpenDialog->FileName.SubString(FileOpenDialog->FileName.Length()-3,4) == AnsiString(".txt") )
        OpenTxt( FileOpenDialog->FileName );
    else if( FileOpenDialog->FileName.SubString(FileOpenDialog->FileName.Length()-3,4) == AnsiString(".bit") )
        OpenBitProject( FileOpenDialog->FileName );
    else
        OpenBin( FileOpenDialog->FileName );

    DrawBitsButtonClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::BitImageEnViewMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
    AnsiString TempString;
    int MouseX;
    int MouseY;


    if( ! (BitsPerOutput && BitSizeX && BitSizeY) )
        return;

    MouseX = BitImageEnView->XScr2Bmp(X);
    MouseY = BitImageEnView->YScr2Bmp(Y);

    MouseX -= ColGap * ( MouseX / ( (BitSizeX+BitGap) * BitsPerOutput + ColGap ) );
    MouseX /= (BitSizeX+BitGap);

    MouseY /= (BitSizeY+BitGap);

// MouseX is bit column, MouseY is bit row

    BitNumberLabel->Caption = MouseX + MouseY*BitsPerRow;

    RowLabel->Caption = MouseY;
    GroupLabel->Caption = MouseX/BitsPerOutput + 1;

    ColLabel->Caption = MouseX % BitsPerOutput;
    if( !(MouseX%BitsPerOutput) )
        ColLabel->Caption = BitsPerOutput;

    FullColLabel->Caption = MouseX;


    if( MouseDownSelecting )
    {
        int FoundRow;
        int FoundCol;
        int BitInRow;
        int RowNumber;
        int i;

        int ViewX = BitImageEnView->ViewX;
        int ViewY = BitImageEnView->ViewY;
        int OffsetX = BitImageEnView->OffsetX;
        int OffsetY = BitImageEnView->OffsetY;
        double ZoomX = BitImageEnView->ZoomX/100.0;
        double ZoomY = BitImageEnView->ZoomY/100.0;

        X -= OffsetX;
        Y -= OffsetY;

        X += ViewX;
        Y += ViewY;

// loop through NumberOfBits, a byte at a time (BitCounter is incremented by inner BitInByteCounter loop)
        for( int BitCounter=0; BitCounter < NumberOfBits; BitCounter++ )
        {
            FoundRow = 0;
            FoundCol = 0;

            BitInRow = BitCounter % BitsPerRow;
            RowNumber = BitCounter / BitsPerRow;

            if( (5 + BitInRow * (BitSizeX+BitGap) + BitInRow/BitsPerOutput * ColGap)*ZoomX <= X && (5 + BitInRow * (BitSizeX+BitGap) + BitInRow/BitsPerOutput * ColGap + BitSizeX)*ZoomX >= X )
                FoundCol = 1;
            if( (5 + RowNumber * (BitSizeY+BitGap))*ZoomY <= Y && (5 + RowNumber * (BitSizeY+BitGap) + BitSizeY)*ZoomY >= Y )
                FoundRow = 1;

            switch( ActionRadioGroup->ItemIndex )
            {
                case 0: // select bit
                    if( FoundRow && FoundCol )
                    {
                        if( MouseDownSelecting-1==mbLeft )
                            BinData[BitCounter] |= ROM_SEL;
                        else if( MouseDownSelecting-1==mbRight )
                            BinData[BitCounter] &= ~ROM_SEL;
                        DrawBitsButtonClick(Sender);
                        return;
                    }
                    break;

                case 1: // sel row
                    if( FoundRow )
                    {
                        for( i=RowNumber*BitsPerRow; i<RowNumber*BitsPerRow+BitsPerRow; i++ )
                        {
                            if( MouseDownSelecting-1==mbLeft )
                                BinData[i] |= ROM_SEL;
                            else if( MouseDownSelecting-1==mbRight )
                                BinData[i] &= ~ROM_SEL;
                        }
                        DrawBitsButtonClick(Sender);
                        return;
                    }
                    break;

                case 2: // sel col
                    if( FoundCol )
                    {
                        for( i=BitInRow; i<NumberOfBits; i+=BitsPerRow )
                        {
                            if( MouseDownSelecting-1==mbLeft )
                                BinData[i] |= ROM_SEL;
                            else if( MouseDownSelecting-1==mbRight )
                                BinData[i] &= ~ROM_SEL;
                        }
                        DrawBitsButtonClick(Sender);
                        return;
                    }
                    break;

                case 3: // set bit
                    if( FoundRow && FoundCol )
                    {
                        if( MouseDownSelecting-1==mbLeft )
                            BinData[BitCounter] |= ROM_BIT;
                        else if( MouseDownSelecting-1==mbRight )
                            BinData[BitCounter] &= ~ROM_BIT;

                        DrawBitsButtonClick(Sender);
                        return;
                    }
                    break;
            }
        }

    }
}
//---------------------------------------------------------------------------
int TMainForm::OpenBin(AnsiString InFilename)
{
    FILE *InFile;              // binary input file
    int InputByte;              // one byte of input data
    int ReadByte;
    int i,n;


// open input bin file
    InFile = fopen( InFilename.c_str(), "rb" );
    if(!InFile)
    {
        Application->MessageBoxA("Unable to open input binary file. Aborting.","Error",MB_OK);
        return 0;
    }

// set MainForm caption to display current filename
    MainForm->Caption = "Bitviewer: "+InFilename;

// seek to end of input file
    fseek( InFile, 0, SEEK_END );

    BinDataLen = ftell( InFile ) * 8;

// seek to StartingBitOffset in input file
//    fseek( InFile, StartingBitOffset/8, SEEK_SET );
    fseek( InFile, 0, SEEK_SET );

    if(BinData)
        delete[] BinData;

    BinData = new unsigned char[BinDataLen];

    memset(BinData,0,BinDataLen);

    for( i=0; i<BinDataLen && (!feof(InFile)) && (!ferror(InFile)); i++ )
    {
        ReadByte = fgetc( InFile );
        if( feof(InFile) || ferror(InFile) )
            break;

        for( n=0; n<8; n++ )
        {
            if( ReadByte & (1<<n) )
                BinData[i*8 + n] = 0x01;
            else
                BinData[i*8 + n] = 0x00;
        }
    }
    BinDataLen = i*8;     // BinDataLen now is # of bits read

    fclose(InFile);


    return BinDataLen;
}
//---------------------------------------------------------------------------
int TMainForm::OpenBitProject(AnsiString InFilename)
{
    FILE *InFile;              // binary input file
    int InputByte;              // one byte of input data
    int ReadByte;
    int i,n;
    char InLine[1024];
    char Keyword[1024];
    char strValue[1024];
    int iValue;
    int HasWarned=0;


// open input bin file
    InFile = fopen( InFilename.c_str(), "rb" );
    if(!InFile)
    {
        Application->MessageBoxA("Unable to open input binary file. Aborting.","Error",MB_OK);
        return 0;
    }

// set MainForm caption to display current filename
    MainForm->Caption = "Bitviewer: "+InFilename;

    BinDataLen = 0;
    while( (!feof(InFile)) && (!ferror(InFile)) )
    {
        fgets( InLine, sizeof(InLine), InFile );        // read input line from .bit file header
        if( ferror(InFile) || feof(InFile) )
            break;

        if( InLine[0] == '\n' )
            break;                  // end of file header reached

//    int BitSizeX;               // X size of each bit in BitImage
//    int BitSizeY;               // Y size of each bit in BitImage
//    int BitGap;                 // size of gaps between bits
//    int ColGap;                 // size of gaps between columns in BitImage
//    int BitsPerRow;             // number of bits on each row
//    int BitsPerOutput;          // number of bits in each column
//    int NumCols;                // number of columns
//    int NumberOfBits;           // total number of bits

        if( sscanf( InLine, "%s = %d", Keyword, &iValue ) != 2 )
        {
            if( !HasWarned)
                Application->MessageBoxA( "The .bit project file header contains unreadable lines.", "WARNING", MB_OK );
            HasWarned=1;
        }

        if( !strcmp( "BitSizeX", Keyword ) )
            MainForm->BitSizeXEdit->Text = AnsiString(iValue);
        else if( !strcmp( "BitSizeY", Keyword ) )
            MainForm->BitSizeYEdit->Text = AnsiString(iValue);
        else if( !strcmp( "BitGap",Keyword ) )
            MainForm->BitGapEdit->Text = AnsiString(iValue);
        else if( !strcmp( "ColGap",Keyword ) )
            MainForm->ColGapEdit->Text = AnsiString(iValue);
        else if( !strcmp( "BitsPerRow",Keyword ) )
            MainForm->BitsPerRowEdit->Text = AnsiString(iValue);
        else if( !strcmp( "BitsPerOutput",Keyword ) )
            MainForm->BitsPerOutputEdit->Text = AnsiString(iValue);
        else if( !strcmp( "BinDataLen",Keyword ) )
            BinDataLen = iValue;
        else if( !strcmp( "ReverseOutputs",Keyword ) )
            ReverseOutputs = iValue;
        else if( !strcmp( "ReverseWithinOutCol",Keyword ) )
            ReverseWithinOutCol = iValue;
        else if( !strcmp( "ROMLayout",Keyword ) )
            ROM_Layout = (rom_layout_type)iValue;
        else
        {
            if( sscanf( InLine, "%s = ", Keyword ) == 1 )
            {
                if( !strcmp( "OutputBitOrder",Keyword ) )
                {
                    OutputBitOrderStrToArray( InLine+strlen(Keyword)+3 );
                }
            }
        }
    }
// read header - until blank line is reached

// seek to end of input file
//    fseek( InFile, 0, SEEK_END );
//    BinDataLen = ftell( InFile ) * 8;
//    fseek( InFile, 0, SEEK_SET );

    if(BinData)
        delete[] BinData;

    BinData = new unsigned char[BinDataLen];
    memset(BinData,0,BinDataLen);

    if( fread( BinData, BinDataLen, 1, InFile ) < 1 )
    {
        Application->MessageBoxA( "The .bit project file contains unreadable data.", "ERROR", MB_OK );
        BinDataLen=0;
        return BinDataLen;
    }

    fclose(InFile);


    return BinDataLen;
}
//---------------------------------------------------------------------------
int TMainForm::OpenTxt(AnsiString InFilename)
{
    FILE *InFile;              // binary input file
    int ReadByte;
    int ByteIdx=0;

// open input bin file
    InFile = fopen( InFilename.c_str(), "rb" );
    if(!InFile)
    {
        Application->MessageBoxA("Unable to open input text file. Aborting.","Error",MB_OK);
        return 0;
    }

// set MainForm caption to display current filename
    MainForm->Caption = "Bitviewer: "+InFilename;

// seek to end of input file
    fseek( InFile, 0, SEEK_END );
    BinDataLen = ftell( InFile );
    fseek( InFile, 0, SEEK_SET );

    if(BinData)
        delete[] BinData;
    BinData = new unsigned char[BinDataLen];
    memset(BinData,0,BinDataLen);

    BitsPerRow = 0;
    BitsPerOutput = 0;
    while( (!feof(InFile)) && (!ferror(InFile)) )
    {
        ReadByte=fgetc(InFile);
        if( feof(InFile) || ferror(InFile) )
            break;

        if( ReadByte == '1' )
            BinData[ByteIdx] = ROM_BIT;
        else if( ReadByte == '0' )
            BinData[ByteIdx] = 0;
        else if( ReadByte == '?' )                      // TODO - support unknown bits '?'
        {
            Application->MessageBoxA( "The input file contains '?' characters.\nLoaded data may be corrupt because of unknown bits.", "WARNING", MB_OK );
            continue;
        }
        else if( ReadByte == '\n' && !BitsPerRow )
        {
            if( ByteIdx )
                BitsPerRow = ByteIdx;
            continue;
        }
        else if( ReadByte == ' ' && !BitsPerOutput )
        {
            if( ByteIdx )
                BitsPerOutput = ByteIdx;
            continue;
        }
        else
            continue;

        ByteIdx++;
    }
    BinDataLen = ByteIdx;

    fclose(InFile);

    BitsPerRowEdit->Text = AnsiString(BitsPerRow);
    if( BitsPerOutput )
        BitsPerOutputEdit->Text = AnsiString(BitsPerOutput);

    return BinDataLen;
}
//---------------------------------------------------------------------------
void TMainForm::SetupVariables(void)
{
// setup variables based on edits on MainForm
    BitSizeX = BitSizeXEdit->Text.ToIntDef(0);
    BitSizeY = BitSizeYEdit->Text.ToIntDef(0);
    BitGap = BitGapEdit->Text.ToIntDef(0);
    ColGap = ColGapEdit->Text.ToIntDef(0);
    BitsPerRow = BitsPerRowEdit->Text.ToIntDef(0);
    BitsPerOutput = BitsPerOutputEdit->Text.ToIntDef(0);

    NumOutputs = BitsPerRow / BitsPerOutput;

    NumberOfBits = BinDataLen;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormShow(TObject *Sender)
{
    DrawBitsButtonClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SaveBitProjButtonClick(TObject *Sender)
{
    if( WorkingDir.Length() && WorkingDir != BitFileSaveDialog->FileName.SubString(1, BitFileSaveDialog->FileName.LastDelimiter( ":\\" ) ) )
    {
        BitFileSaveDialog->InitialDir=WorkingDir;
        BitFileSaveDialog->FileName="";
    }
    if(BitFileSaveDialog->Execute() != mrOk)
        return;
    WorkingDir = BitFileSaveDialog->FileName.SubString(1, BitFileSaveDialog->FileName.LastDelimiter( ":\\" ) );


    SaveBitProject( BitFileSaveDialog->FileName );
}
//---------------------------------------------------------------------------
int TMainForm::SaveBitProject(AnsiString BinFilename)
{
    FILE *BinFile;              // binary input file
    int InputByte;              // one byte of input data
    int i;
    char OutputOrderStr[1024];


    if(!BinDataLen)
        return 0;

    BinFile = fopen( BinFilename.c_str(), "wb" );
    if(!BinFile)
    {
        Application->MessageBoxA("Unable to open output binary file. Cannot save.","Error",MB_OK);
        return 0;
    }

// set MainForm caption to display current filename
    MainForm->Caption = "Bitviewer: "+BinFilename;

    SetupVariables();
    fprintf(BinFile,"BitSizeX = %d\n",BitSizeX);
    fprintf(BinFile,"BitSizeY = %d\n",BitSizeY);
    fprintf(BinFile,"BitGap = %d\n",BitGap);
    fprintf(BinFile,"ColGap = %d\n",ColGap);
    fprintf(BinFile,"BitsPerRow = %d\n",BitsPerRow);
    fprintf(BinFile,"BitsPerOutput = %d\n",BitsPerOutput);
    fprintf(BinFile,"ReverseOutputs = %d\n",ReverseOutputs);
    fprintf(BinFile,"ReverseWithinOutCol = %d\n",ReverseWithinOutCol);
    fprintf(BinFile,"ROMLayout = %d\n",ROM_Layout);
    fprintf(BinFile,"OutputBitOrder = %s\n",OutputBitOrderArrayToStr( OutputOrderStr, sizeof(OutputOrderStr) ));

    fprintf(BinFile,"BinDataLen = %d\n",BinDataLen);

    fprintf(BinFile,"\n");

    fwrite(BinData, BinDataLen, 1, BinFile);


    fclose(BinFile);


    return BinDataLen;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SaveImageButtonClick(TObject *Sender)
{
    if( WorkingDir.Length() && WorkingDir != JpgSaveDialog->FileName.SubString(1, JpgSaveDialog->FileName.LastDelimiter( ":\\" ) ) )
    {
        JpgSaveDialog->InitialDir=WorkingDir;
        JpgSaveDialog->FileName="";
    }
    if(JpgSaveDialog->Execute() != mrOk)
        return;
    WorkingDir = JpgSaveDialog->FileName.SubString(1, JpgSaveDialog->FileName.LastDelimiter( ":\\" ) );

    BitImageEnIO->SaveToFile(JpgSaveDialog->FileName);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::BitImageEnViewMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int FoundRow;
    int FoundCol;
    int BitInRow;
    int RowNumber;
    int i;


    int ViewX = BitImageEnView->ViewX;
    int ViewY = BitImageEnView->ViewY;
    int OffsetX = BitImageEnView->OffsetX;
    int OffsetY = BitImageEnView->OffsetY;
    double ZoomX = BitImageEnView->ZoomX/100.0;
    double ZoomY = BitImageEnView->ZoomY/100.0;

    X -= OffsetX;
    Y -= OffsetY;

    X += ViewX;
    Y += ViewY;

// loop through NumberOfBits, a byte at a time (BitCounter is incremented by inner BitInByteCounter loop)
    for( int BitCounter=0; BitCounter < NumberOfBits ; BitCounter++ )
    {
            FoundRow = 0;
            FoundCol = 0;

            BitInRow = BitCounter % BitsPerRow;
            RowNumber = BitCounter / BitsPerRow;

            if( (5 + BitInRow * (BitSizeX+BitGap) + BitInRow/BitsPerOutput * ColGap)*ZoomX <= X && (5 + BitInRow * (BitSizeX+BitGap) + BitInRow/BitsPerOutput * ColGap + BitSizeX)*ZoomX >= X )
                FoundCol = 1;
            if( (5 + RowNumber * (BitSizeY+BitGap))*ZoomY <= Y && (5 + RowNumber * (BitSizeY+BitGap) + BitSizeY)*ZoomY >= Y )
                FoundRow = 1;

            switch( ActionRadioGroup->ItemIndex )
            {
                case 0: // select bit
                    if( FoundRow && FoundCol )
                    {
                        if( Button==mbLeft )
                            BinData[BitCounter] |= ROM_SEL;
                        else if( Button==mbRight )
                            BinData[BitCounter] &= ~ROM_SEL;
                        DrawBitsButtonClick(Sender);
                        MouseDownSelecting=1+Button;
                        return;
                    }
                    break;

                case 1: // sel row
                    if( FoundRow )
                    {
                        for( i=RowNumber*BitsPerRow; i<RowNumber*BitsPerRow+BitsPerRow; i++ )
                        {
                            if( Button==mbLeft )
                                BinData[i] |= ROM_SEL;
                            else if( Button==mbRight )
                                BinData[i] &= ~ROM_SEL;
                        }
                        DrawBitsButtonClick(Sender);
                        MouseDownSelecting=1+Button;
                        return;
                    }
                    break;

                case 2: // sel col
                    if( FoundCol )
                    {
                        for( i=BitInRow; i<NumberOfBits; i+=BitsPerRow )
                        {
                            if( Button==mbLeft )
                                BinData[i] |= ROM_SEL;
                            else if( Button==mbRight )
                                BinData[i] &= ~ROM_SEL;
                        }
                        DrawBitsButtonClick(Sender);
                        MouseDownSelecting=1+Button;
                        return;
                    }
                    break;

                case 3: // set bit
                    if( FoundRow && FoundCol )
                    {
                        if( Button==mbLeft )
                            BinData[BitCounter] |= ROM_BIT;
                        else if( Button==mbRight )
                            BinData[BitCounter] &= ~ROM_BIT;

                        DrawBitsButtonClick(Sender);
                        MouseDownSelecting=1+Button;
                        return;
                    }
                    break;
            }
    }
    MouseDownSelecting=0;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormDestroy(TObject *Sender)
{
// write .ini file to save form setup (x/y position etc)
    TIniFile *BitviewerIni;
    char OutputOrderStr[1024];



    SetupVariables();

    BitviewerIni = new TIniFile( ChangeFileExt( Application->ExeName, ".ini" ) );
    BitviewerIni->WriteInteger( "BitviewerForm", "Top", Top );          // save top/left of form
    BitviewerIni->WriteInteger( "BitviewerForm", "Left", Left );
    BitviewerIni->WriteInteger( "BitviewerForm", "Width", Width );      // save width/height of form
    BitviewerIni->WriteInteger( "BitviewerForm", "Height", Height );

    BitviewerIni->WriteInteger( "BitviewerForm", "BitSizeX", BitSizeXEdit->Text.ToIntDef(4) );
    BitviewerIni->WriteInteger( "BitviewerForm", "BitSizeY", BitSizeYEdit->Text.ToIntDef(4) );
    BitviewerIni->WriteInteger( "BitviewerForm", "BitGap", BitGapEdit->Text.ToIntDef(2) );
    BitviewerIni->WriteInteger( "BitviewerForm", "ColGap", ColGapEdit->Text.ToIntDef(4) );
    BitviewerIni->WriteInteger( "BitviewerForm", "BitsPerRow", BitsPerRowEdit->Text.ToIntDef(1024) );
    BitviewerIni->WriteInteger( "BitviewerForm", "BitsPerOutput", BitsPerOutputEdit->Text.ToIntDef(32) );

    BitviewerIni->WriteString( "BitviewerForm", "WorkingDir", WorkingDir );

    BitviewerIni->WriteInteger( "Export", "ReverseOutputs", ReverseOutputs );
    BitviewerIni->WriteInteger( "Export", "ReverseWithinOutCol", ReverseWithinOutCol );
    BitviewerIni->WriteInteger( "Export", "ROMLayout", ROM_Layout ); 
    BitviewerIni->WriteString( "Export", "OutputBitOrder", OutputBitOrderArrayToStr( OutputOrderStr, sizeof(OutputOrderStr) ) );

    delete BitviewerIni;


    if( BinData )
    {
        delete[] BinData;
        BinData=NULL;
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::InvertSelButtonClick(TObject *Sender)
{
    SetupVariables();


// loop through NumberOfBits, a byte at a time (BitCounter is incremented by inner BitInByteCounter loop)
    for( int BitCounter=0; BitCounter < NumberOfBits; BitCounter++ )
    {
// read input byte from bin file
        if( BinData[BitCounter] & ROM_SEL )
            BinData[BitCounter] ^= ROM_BIT;
    }
    DrawBitsButtonClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ClearSelButtonClick(TObject *Sender)
{
    SetupVariables();

    for( int BitCounter=0; BitCounter < NumberOfBits ; BitCounter++ )
        BinData[BitCounter] &= ~ROM_SEL;

    DrawBitsButtonClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SelAllButtonClick(TObject *Sender)
{
    SetupVariables();

    for( int BitCounter=0; BitCounter < NumberOfBits ; BitCounter++ )
        BinData[BitCounter] |= ROM_SEL;

    DrawBitsButtonClick(Sender);
}
//---------------------------------------------------------------------------
int __fastcall TMainForm::OutputBitOrderStrToArray( char *OutputBitOrderStr )
{
    char *ParseIdx;
    char *EndIdx=NULL;
    int i,n;


    if( !OutputBitOrderStr || !OutputBitOrderStr[0] )
        return 0;     // NULL or empty string

    for( i=0, ParseIdx=OutputBitOrderStr ; i<MAX_ROM_OUTPUTS && ParseIdx[0] ; i++, ParseIdx=EndIdx )
    {
        OutputOrder[i] = strtoul( ParseIdx, &EndIdx, 0 );
        if( !EndIdx )   // error - EndIdx should point to char that stopped the scan
            break;
    }
// i should = NumOutputs

    return i;
}
//---------------------------------------------------------------------------
char * __fastcall TMainForm::OutputBitOrderArrayToStr( char *OutputBitOrderStr, int MaxLen )
{
    SetupVariables();
    
    OutputBitOrderStr[0] = '\0';
    for( int i=0; i<NumOutputs; i++ )
        snprintf( OutputBitOrderStr+strlen(OutputBitOrderStr), MaxLen-strlen(OutputBitOrderStr), "%s%d", i?" ":"", OutputOrder[i] );

    return OutputBitOrderStr;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormCreate(TObject *Sender)
{
// read .ini file to setup form (x/y position etc)
    TIniFile *BitviewerIni;

    BitviewerIni = new TIniFile( ChangeFileExt( Application->ExeName, ".ini" ) );
    Top     = BitviewerIni->ReadInteger( "BitviewerForm", "Top", 0 );
    Left    = BitviewerIni->ReadInteger( "BitviewerForm", "Left", 0 );
    Width   = BitviewerIni->ReadInteger( "BitviewerForm", "Width", Width );
    Height  = BitviewerIni->ReadInteger( "BitviewerForm", "Height", Height );

    BitSizeXEdit->Text = BitviewerIni->ReadInteger( "BitviewerForm", "BitSizeX", BitSizeXEdit->Text.ToIntDef(4) );
    BitSizeYEdit->Text = BitviewerIni->ReadInteger( "BitviewerForm", "BitSizeY", BitSizeYEdit->Text.ToIntDef(4) );
    BitGapEdit->Text = BitviewerIni->ReadInteger( "BitviewerForm", "BitGap", BitGapEdit->Text.ToIntDef(2) );
    ColGapEdit->Text = BitviewerIni->ReadInteger( "BitviewerForm", "ColGap", ColGapEdit->Text.ToIntDef(4) );
    BitsPerRowEdit->Text = BitviewerIni->ReadInteger( "BitviewerForm", "BitsPerRow", BitsPerRowEdit->Text.ToIntDef(1024) );
    BitsPerOutputEdit->Text = BitviewerIni->ReadInteger( "BitviewerForm", "BitsPerOutput", BitsPerOutputEdit->Text.ToIntDef(32) );

    WorkingDir = BitviewerIni->ReadString( "BitviewerForm", "WorkingDir", "" );

    ReverseOutputs = BitviewerIni->ReadInteger( "Export", "ReverseOutputs", 0 );
    ReverseWithinOutCol = BitviewerIni->ReadInteger( "Export", "ReverseWithinOutCol", 0 );
    ROM_Layout = (rom_layout_type)BitviewerIni->ReadInteger( "Export", "ROMLayout", 0 );
    OutputBitOrderStrToArray( BitviewerIni->ReadString( "Export", "OutputBitOrder", "0 1 2 3 4 5 6 7" ).c_str() );

    delete BitviewerIni;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::BitImageEnViewMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    MouseDownSelecting=0;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::BitsPerRowEditKeyPress(TObject *Sender,
      char &Key)
{
    if( Key=='\r' )
    {
        DrawBitsButtonClick(NULL);
        Key = 0;
    }
}
//---------------------------------------------------------------------------
int __fastcall TMainForm::ExportBin()
{
    int BitCounter;
    unsigned char *ExportData;


    if( BinDataLen < 8 )
        return 0;     // nothing to export

    SetupVariables();


//    ByteData=NULL;    // byte output (used for HexForm and save .bin to disk)
//    ByteDataLen=0;

    ExportData = new unsigned char[ BinDataLen/8 ];
    memset( ExportData, 0, BinDataLen/8 );

    for( BitCounter=0; BitCounter<BinDataLen; BitCounter++ )
    {
        int RowIdx = BitCounter/BitsPerRow;                 // current row #
        int BitWithinRow = BitCounter%BitsPerRow;           // bit # within this row [ie: 0..1023]
        int BitWithinOutCol = BitWithinRow / NumOutputs;    // bit # within this output column [ie: 0..31]
        int BitOutputIdx = BitWithinRow % NumOutputs;       // output column this bit is in [ie: 0..31]
        int ExportFromBitPos;
        int FirstColOutputBit;

        BitOutputIdx = OutputOrder[BitOutputIdx];

        if( ReverseOutputs )
            BitOutputIdx = NumOutputs - BitOutputIdx - 1;
        if( ReverseWithinOutCol )
            BitWithinOutCol = BitsPerOutput - BitWithinOutCol - 1;

        if( BinData[ (RowIdx*BitsPerRow) + (BitOutputIdx*BitsPerOutput)+BitWithinOutCol ] & ROM_BIT )
                ExportData[BitCounter/8] |= (1<<(BitCounter%8));
    }

    // flip endianness
    if( Endianness == 1 )
    {
        int BytesInOutput;
        int i;
        int n;
        BytesInOutput = ((BitsPerRow / BitsPerOutput) + 7) / 8;
        for( i=0; i<ByteDataLen; i+=BytesInOutput )
        {
            for( n=0; n<BytesInOutput/2; n++ )
            {
                unsigned char tmp;

                tmp = ExportData[i+n];
                ExportData[i+n] = ExportData[i+BytesInOutput-n-1];
                ExportData[i+BytesInOutput-n-1] = tmp;
            }
        }
    }

    if( ( ByteData && ByteDataLen==(BinDataLen/8) && memcmp(ExportData,ByteData,ByteDataLen) ) ||
        ( !ByteData ) || (ByteDataLen!=(BinDataLen/8)) )
    {
        delete[] ByteData;
        ByteData = ExportData;
        ByteDataLen = BinDataLen/8;
        return ByteDataLen;
    }

    // no change to BinData[]
    delete[] ExportData;

    return 0;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ExportOptsButtonClick(TObject *Sender)
{
    ExportForm->Show();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::Byteviewhex1Click(TObject *Sender)
{
    ExportBin();
    HexForm->UpdateBin(ByteData,ByteDataLen);
    HexForm->Show();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::Selectpattern1Click(TObject *Sender)
{
    unsigned char *pat;
    unsigned char *SelPattern;
    int SelPatternLen=0;
    int i,n;
    int pat_idx;
    int col_idx,row_idx;


    if( SelPatForm->ShowModal() != mrOk )
        return;

    if( SelPatForm->PatEdit->Text.Length() < 1 )
    return;

    pat = (unsigned char *)calloc( SelPatForm->PatEdit->Text.Length()+1, 1 );
    if( !pat )
        return;
    strcpy( pat, SelPatForm->PatEdit->Text.c_str() );

        // the SelPattern will for sure fit in this space
    SelPatternLen = strlen(pat)*4;

    SelPattern = (unsigned char *)calloc( SelPatternLen, 1 );
    if( !SelPattern )
        return;

    for( i=0,pat_idx=0; i<strlen(pat); i++ )
    {
        unsigned char digit = pat[i];
        if( digit >= '0' && digit <= '9' )
            digit-='0';
        else if( digit >= 'a' && digit <= 'f' )
            digit-=('a'-0xA);
        else if( digit >= 'A' && digit <= 'F' )
            digit-=('a'-0xA);
        else
            continue;

        for( n=0; n<4; n++,pat_idx++ )
        {
            if( digit & (1<<(3-n)) )
                SelPattern[pat_idx] = 1;
        }
    }

    SelPatternLen = pat_idx;      // length of select pattern, in bits


    SetupVariables();

// calculate total number of pixels in BitImage
//    BitImageWidth = BitsPerRow * (BitSizeX+BitGap) + (ColGap * ( BitsPerRow / BitsPerOutput - 1 ) ) - BitGap;
//    BitImageHeight = (NumberOfBits / BitsPerRow) * (BitSizeX+BitGap) - BitGap;

    for( col_idx=0; col_idx<BitsPerRow; col_idx++ )
    {
        if( SelPattern[ (col_idx%SelPatternLen) ] )
        {
            for( row_idx=0; row_idx<NumberOfBits/BitsPerRow; row_idx++ )
            {
                BinData[row_idx*BitsPerRow + col_idx] |= ROM_SEL;
            }
        }
    }

    DrawBitsButtonClick(Sender);

    free(SelPattern);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SaveBinButtonClick(TObject *Sender)
{
    int BitCounter;
    FILE *ExportFile;


    if( !BinDataLen )
    {
        Application->MessageBoxA( "No data to export.", "ERROR", MB_OK );
        return;
    }

    if( WorkingDir.Length() && WorkingDir != BinSaveDialog->FileName.SubString(1, BinSaveDialog->FileName.LastDelimiter( ":\\" ) ) )
    {
        BinSaveDialog->InitialDir=WorkingDir;
        BinSaveDialog->FileName="";
    }
    if(BinSaveDialog->Execute() != mrOk)
        return;
    WorkingDir = BinSaveDialog->FileName.SubString(1, BinSaveDialog->FileName.LastDelimiter( ":\\" ) );

    ExportFile = fopen( BinSaveDialog->FileName.c_str(), "wb" );
    if( !ExportFile )
    {
        Application->MessageBoxA( "Unable to open .bin file to write!", "ERROR", MB_OK );
        return;
    }

    ExportBin();
    fwrite(ByteData, ByteDataLen, 1, ExportFile);

    fclose(ExportFile);

    return;
}
//---------------------------------------------------------------------------

