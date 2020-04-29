//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PermFrm.h"
#include "MainFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPermForm *PermForm;
//---------------------------------------------------------------------------
__fastcall TPermForm::TPermForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TPermForm::EveryFourthButtonClick(TObject *Sender)
{
    unsigned char *NewBinData=NULL;         // NewBinData[] where permuted bits will be put
    int DestOfs;            // current offset into destination buffer


// make sure there is some data loaded
    if( !MainForm->BinDataLen )
        return;

    MainForm->SetupVariables();

// allocate space for NewBinData[]
    NewBinData = new char[ MainForm->BinDataLen ];

// clear all NewBinData[] to 0
    for(int counter=0; counter< MainForm->BinDataLen; counter++)
        NewBinData[ counter] = 0;

    DestOfs = 0;

// loop through data for permutations
    for(int ByteCounter=0; ByteCounter < MainForm->BinDataLen; ByteCounter++)
    {
// loop through 8 bits of this byte
        for(int BitCounter=0; BitCounter < 8; BitCounter++)
        {
            if( GetBit( MainForm->BinData[ ByteCounter ], BitCounter ) )
            {
//                NewBinData[ DestOfs + BitCounter*8 + (ByteCounter%64)/8 ] = SetBit( NewBinData[ DestOfs + BitCounter*8 + (ByteCounter%64)/8 ], ByteCounter % 8);

//                NewBinData [ DestOfs + (7-BitCounter*8)  + 7- (ByteCounter%64)/8  ] = SetBit( NewBinData[ DestOfs + (7-BitCounter*8) + 7 - (ByteCounter%64)/8 ], ByteCounter % 8 );

                NewBinData[ DestOfs + BitCounter*8 + (ByteCounter%64)/8 ] = SetBit( NewBinData[ DestOfs + BitCounter*8 + (ByteCounter%64)/8 ], 7-ByteCounter%8 );
            }

        }

        if( !( (ByteCounter+1) % 64) )
            DestOfs+=64;
        }

// remove the old BinData[]
    delete[] MainForm->BinData;

// now set MainForm->BinData to point to the NewBinData[]
    MainForm->BinData = NewBinData;

    MainForm->DrawBitsButtonClick(NULL);
}
//---------------------------------------------------------------------------
unsigned char SetBit(unsigned char Data,int Bit)
{
    return ( Data | (0x80>>Bit) );
}
//---------------------------------------------------------------------------
unsigned char ClearBit(unsigned char Data,int Bit)
{
    return ( Data & (~(0x80>>Bit)) );
}
//---------------------------------------------------------------------------
unsigned char GetBit(unsigned char Data,int Bit)
{
    return ( Data & (0x80>>Bit) );
}
//---------------------------------------------------------------------------
void __fastcall TPermForm::OneBitPerColButtonClick(TObject *Sender)
{
    unsigned char *NewBinData=NULL;         // NewBinData[] where permuted bits will be put
    int DestOfs;            // current offset into destination buffer


// make sure there is some data loaded
    if( !MainForm->BinDataLen )
        return;

    MainForm->SetupVariables();

// allocate space for NewBinData[]
    NewBinData = new char[ MainForm->BinDataLen ];

// clear all NewBinData[] to 0
    for(int counter=0; counter< MainForm->BinDataLen; counter++)
        NewBinData[ counter] = 0;

    DestOfs = 0;

// loop through data for permutations
    for(int ByteCounter=0; ByteCounter < MainForm->BinDataLen; ByteCounter++)
    {
// loop through 8 bits of this byte
        for(int BitCounter=0; BitCounter < 8; BitCounter++)
        {
            if( GetBit( MainForm->BinData[ ByteCounter ], BitCounter ) )
            {
                NewBinData[ DestOfs + BitCounter ] = SetBit( NewBinData[ DestOfs+BitCounter ], ByteCounter % 8 );
            }

        }

        if( !( (ByteCounter+1) % 8) )
            DestOfs += 8;
    }

// remove the old BinData[]
    delete[] MainForm->BinData;

// now set MainForm->BinData to point to the NewBinData[]
    MainForm->BinData = NewBinData;

    MainForm->DrawBitsButtonClick(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TPermForm::Pattern1122ButtonClick(TObject *Sender)
{
    unsigned char *NewBinData=NULL;         // NewBinData[] where permuted bits will be put
    int DestOfs;            // current offset into destination buffer


// make sure there is some data loaded
    if( !MainForm->BinDataLen )
        return;

    MainForm->SetupVariables();

// allocate space for NewBinData[]
    NewBinData = new char[ MainForm->BinDataLen ];

// clear all NewBinData[] to 0
    for(int counter=0; counter< MainForm->BinDataLen; counter++)
        NewBinData[ counter] = 0;

    DestOfs = 0;

// loop through data for permutations
    for(int ByteCounter=0; ByteCounter < MainForm->BinDataLen; ByteCounter++)
    {
// loop through 8 bits of this byte
        for(int BitCounter=0; BitCounter < 8; BitCounter++)
        {
            if( GetBit( MainForm->BinData[ ByteCounter ], BitCounter ) )
            {
/*
                if( (ByteCounter % (MainForm->BitsPerCol/8) ) ==  // right half of column

                if( (ByteCounter % (MainForm->BitsPerRow/8)) < 16 )    // part of 11221212
                {
                    if(ByteCounter % 2)
                    {   // right half of column
                    }
                    else
                    {   // left half of column
                        NewBinData[ DestOfs + 0
                    }
                }

                NewBinData[ DestOfs + BitCounter ] = SetBit( NewBinData[ DestOfs+BitCounter ], ByteCounter % 8 );
*/
            }

        }

        if( !( (ByteCounter+1) % 8) )
            DestOfs += 8;
    }

// remove the old BinData[]
    delete[] MainForm->BinData;

// now set MainForm->BinData to point to the NewBinData[]
    MainForm->BinData = NewBinData;

    MainForm->DrawBitsButtonClick(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TPermForm::Every4ReversedButtonClick(TObject *Sender)
{
    unsigned char *NewBinData=NULL;         // NewBinData[] where permuted bits will be put
    int DestOfs;            // current offset into destination buffer


// make sure there is some data loaded
    if( !MainForm->BinDataLen )
        return;

    MainForm->SetupVariables();

// allocate space for NewBinData[]
    NewBinData = new char[ MainForm->BinDataLen ];

// clear all NewBinData[] to 0
    for(int counter=0; counter< MainForm->BinDataLen; counter++)
        NewBinData[ counter] = 0;

    DestOfs = 0;

// loop through data for permutations
    for(int ByteCounter=0; ByteCounter < MainForm->BinDataLen; ByteCounter++)
    {
// loop through 8 bits of this byte
        for(int BitCounter=0; BitCounter < 8; BitCounter++)
        {
            if( GetBit( MainForm->BinData[ ByteCounter ], 7-BitCounter ) )
            {

                NewBinData [ DestOfs + BitCounter*8  +  (ByteCounter%64)/8 ] = SetBit( NewBinData[ DestOfs +  ( 511- (BitCounter*8 + (ByteCounter%64)/8) ) ], ByteCounter % 8 );
            }

        }

        if( !( (ByteCounter+1) % 64) )
            DestOfs+=64;
    }

// remove the old BinData[]
    delete[] MainForm->BinData;

// now set MainForm->BinData to point to the NewBinData[]
    MainForm->BinData = NewBinData;

    MainForm->DrawBitsButtonClick(NULL);
}
//---------------------------------------------------------------------------


