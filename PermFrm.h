//---------------------------------------------------------------------------

#ifndef PermFrmH
#define PermFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TPermForm : public TForm
{
__published:	// IDE-managed Components
    TButton *EveryFourthButton;
    TButton *OneBitPerColButton;
    TButton *Pattern1122Button;
    TButton *Every4ReversedButton;
    void __fastcall EveryFourthButtonClick(TObject *Sender);
    void __fastcall OneBitPerColButtonClick(TObject *Sender);
    void __fastcall Pattern1122ButtonClick(TObject *Sender);
    void __fastcall Every4ReversedButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TPermForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
unsigned char SetBit(unsigned char Data,int Bit);
unsigned char ClearBit(unsigned char Data,int Bit);
unsigned char GetBit(unsigned char Data,int Bit);
//---------------------------------------------------------------------------
extern PACKAGE TPermForm *PermForm;
//---------------------------------------------------------------------------
#endif
