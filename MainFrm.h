//---------------------------------------------------------------------------

#ifndef MainFrmH
#define MainFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ieview.hpp"
#include "imageenview.hpp"
#include <Menus.hpp>
#include <Dialogs.hpp>
#include <stdio.h>
#include "imageenio.hpp"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
// maximum # of ROM outputs supported (typical # of outputs is 8/16/32/64)
#define MAX_ROM_OUTPUTS 1024
//---------------------------------------------------------------------------
// bitmasks for unsigned char BinData[]
#define ROM_BIT         1
#define ROM_UNK         2
#define ROM_SEL         4
#define ROM_SELB        8
#define ROM_SELPREV     0x10
//---------------------------------------------------------------------------
typedef enum {
    ROM_STANDARD,
} rom_layout_type;
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
    TImageEnView *BitImageEnView;
    TMainMenu *MainMenu1;
    TMenuItem *File1;
    TMenuItem *Exit1;
    TEdit *BitSizeXEdit;
    TEdit *BitSizeYEdit;
    TEdit *BitsPerRowEdit;
    TEdit *BitsPerOutputEdit;
    TEdit *ColGapEdit;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label5;
    TLabel *Label6;
    TLabel *Label7;
    TOpenDialog *FileOpenDialog;
    TButton *OpenBinFileButton;
    TEdit *BitGapEdit;
    TLabel *Label8;
    TLabel *Label9;
    TLabel *Label11;
    TLabel *Label12;
    TLabel *Label13;
    TLabel *BitNumberLabel;
    TLabel *RowLabel;
    TLabel *ColLabel;
    TLabel *GroupLabel;
    TLabel *Label14;
    TLabel *FullColLabel;
    TButton *SaveBitProjButton;
    TSaveDialog *BitFileSaveDialog;
    TButton *SaveImageButton;
    TSaveDialog *JpgSaveDialog;
    TImageEnIO *BitImageEnIO;
    TRadioGroup *ActionRadioGroup;
    TButton *InvertSelButton;
    TButton *ClearSelButton;
    TButton *SelAllButton;
    TMenuItem *Openfile1;
    TMenuItem *SaveBin1;
    TMenuItem *SaveImage1;
    TMenuItem *Edit1;
    TMenuItem *SelectAll1;
    TMenuItem *Unselect1;
    TMenuItem *Bits1;
    TMenuItem *Invertselectedbits1;
    TButton *ExportOptsButton;
    TSaveDialog *BinSaveDialog;
    TMenuItem *Exportbinary1;
    TMenuItem *View1;
    TMenuItem *Byteviewhex1;
    TMenuItem *Selectpattern1;
    TButton *SaveBinButton;
    TButton *ByteViewButton;
    TMenuItem *Exportoptions1;
    void __fastcall Exit1Click(TObject *Sender);
    void __fastcall DrawBitsButtonClick(TObject *Sender);
    void __fastcall OpenBinFileButtonClick(TObject *Sender);
    void __fastcall BitImageEnViewMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall SaveBitProjButtonClick(TObject *Sender);
    void __fastcall SaveImageButtonClick(TObject *Sender);
    void __fastcall BitImageEnViewMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall InvertSelButtonClick(TObject *Sender);
    void __fastcall ClearSelButtonClick(TObject *Sender);
    void __fastcall SelAllButtonClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall BitImageEnViewMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall BitsPerRowEditKeyPress(TObject *Sender, char &Key);
    void __fastcall ExportOptsButtonClick(TObject *Sender);
    int __fastcall OutputBitOrderStrToArray( char *OutputBitOrderStr );
    char * __fastcall OutputBitOrderArrayToStr( char *OutputBitOrderStr, int MaxLen );
    void __fastcall Byteviewhex1Click(TObject *Sender);
    int __fastcall ExportBin();
    void __fastcall Selectpattern1Click(TObject *Sender);
    void __fastcall SaveBinButtonClick(TObject *Sender);
private:	// User declarations

public:		// User declarations
    __fastcall TMainForm(TComponent* Owner);

    void SetupVariables(void);
    int OpenBin(AnsiString BinFilename);
    int OpenBitProject(AnsiString BinFilename);
    int OpenTxt(AnsiString InFilename);
    int SaveBitProject(AnsiString BinFilename);


    int BitSizeX;               // X size of each bit in BitImage
    int BitSizeY;               // Y size of each bit in BitImage
    int BitGap;                 // size of gaps between bits
    int ColGap;                 // size of gaps between columns in BitImage
    int BitsPerRow;             // number of bits on each row
    int BitsPerOutput;          // number of bits in each column
    int NumOutputs;             // number of columns
    int NumberOfBits;           // total number of bits

    int Endianness;

    // export options
    int ReverseOutputs;
    int ReverseWithinOutCol;
    rom_layout_type ROM_Layout;

    int BitImageWidth;          // total size of BitImage
    int BitImageHeight;
    int BinFileLen;

    unsigned char *BinData;     // BinData[]
    int BinDataLen;

    int OutputOrder[MAX_ROM_OUTPUTS];

    unsigned char *ByteData;    // byte output (used for HexForm and save .bin to disk)
    int ByteDataLen;

    int MouseDownSelecting;     // is mouse button down, selecting bits

    AnsiString WorkingDir;
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
