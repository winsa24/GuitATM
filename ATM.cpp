//
//  Guit GUI Code Tester
//  Copyright © 2022 Eric Lecolinet. All rights reserved.
//  http://www.telecom-paris.fr/~elc
//

#include <guit.hpp>
#include <gcodereader.hpp>
#include <gcodeblock.hpp>
#include <ginspector.hpp>
using namespace guit;
using namespace std;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

class ATM : public GWindow {
public:
  ATM();
  ~ATM();
  void readScript(GString const& filename);
  void writeScript(GString const& filename);
  void build();

private:
  GBool changed;
  GText errors, filename;
  gptr<class GCodeBlock> codeblock;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

ATM::~ATM() {
  auto inspector = GInspector::inspector();
  inspector->setInspectedCB(nullptr);
  inspector->setInspectingCB(nullptr);
}

ATM::ATM() : GWindow("#code-edit") {
  GApp()
    << "#screen {size: 30ch 15ch; border: rounded dashed}"
    << "#keyboard {size: 80ch 12ch}"
    << ".window {size:80ch 40ch}"
    << ".label {font: bold 14px Arial,sans-serif; format: 2 2 0; "
    "                   size: 4ch auto; align: center center; border: rounded}"
    << ".nobtn Button {background: #1a0000; color: white;}"
    << ".sidebtn Button {background: #000000;}"
    
    << ".clock .buttonbox {size: auto 7ch; align: center stretch}"
    << ".label .buttonbox .set-btn {size:6ch; font:bold}";

    auto userPsw = Text("13579");

    auto login = Bool(false);
    auto hasCard = Bool(false);
    auto depos = Bool(false);
    auto withd = Bool(false);
    
    auto mesg = Label("Welcome");
    auto psw = Text();
    auto err = Text();
    auto sideTextL = VBox();
    auto sideTextR = VBox();
    
    auto screen = Box("#screen {flow: column; align: stretch stretch;}")
        << mesg
        << psw
        << (Label() << err)
        << (HBox("{flow: row; align: stretch end;}")
            << sideTextL
            << Box()
            << sideTextR
            );
   
    
    auto bg = Background("pink");
    auto bd = Border("rounded");
    
    auto en = Enabled(false);
    
    auto wrongCount = Int(0);
    auto cashBox = Box();
    auto cashCount = Int();
    
    auto interactMoney = [=](auto e){
        
    };
    auto cancelBtn = [=](auto e){
        sideTextL->removeChildren();
        sideTextR->removeChildren();
        sideTextL << "Deposit";
        sideTextR << "Withdraw";
        *depos = false;
        *withd = false;
    };
    auto confirmBtn = [=](auto e){
        *en = true;
        sideTextL->removeChildren();
        sideTextR->removeChildren();
        sideTextL << "Exit";
        sideTextR << "Continue";
    };
    auto depositMoney = [=](auto){
        if(*depos == false){
            *depos = true;
            *mesg = "deposit";
            cashBox << bg << bd << interactMoney;
            sideTextL->removeChildren();
            sideTextR->removeChildren();
            sideTextL << "Cancel";
            sideTextR << "Confirm";
        }
    };
    auto withdrawMoney = [=](auto e){
        *withd = true;
        screen->remove(psw);
        *mesg = "Select the amount of money";
        sideTextL->removeChildren();
        sideTextR->removeChildren();
        sideTextL << "10" << "20" << "50" << "Cancel";
        sideTextR << "100" << "300" << "Others" << "";
    };
    
    
  // main box
  *this
    << QuitDialog("Quit","Changes will be lost", &changed)
    << (VBox(".window {align: stretch stretch;}")
//        << On(~hasCard == false)  / (*en = true)
        << On(~wrongCount > 3)  / [en] {*en = false;}
        << Label(".label ATM ")
        << (HBox()
            << (VBox()
                << (HBox("{align: stretch end;}")
                    << (VBox()
                        << On(~login == true) / Enabled(true)
                        << On(~login == false) / Enabled(false)
                        << (Button()<< depositMoney) << Button() << Button()
                        << (Button() << Guard(~depos == true)/cancelBtn) //TODO::
                        )
                    << screen
                    << (VBox()
                        << On(~login == true) / Enabled(true)
                        << On(~login == false) / Enabled(false)
                        << (Button("{#withdraw}") << withdrawMoney) << Button("") << Button()
                        << (Button("") << On(~depos == true || ~withd == true)/confirmBtn)) //TODO::
                        )
                << (VBox()
                    << Label(".label Cash")
                    << cashBox
                    )
                )
            << (VBox()
                << (Button(".label RECEIPT")
                    << en
                    << On.click / [=](GMouseEvent* e){ *mesg = "Print RECEIPT"; }
                    )
                << (Button(".label CARD")
                    << On.click
                    / [=]{ *hasCard = true; *mesg = "<color=green> Card Inserted"; }

                    // waits for 1 second whitout blocking the event loop
                    / SleepFor(2000)

                    // done once the delay is over
                    / [=]{ *mesg = "<color=blue> Enter Password"; }
                        << Box()
                   )
                )
            )
        << (HBox("#keyboard {align: stretch stretch;}")
            << (Box("#noboard {flow: grid 3}")
                << [=](auto e){ if(e->child()->textValue() != "*" && e->child()->textValue() != "\\#" )*psw += e->child()->textValue();}
//                << Guard(~auth == true)   / Enabled(true)
//                << Guard(~idle == true)   / Enabled(false)
                << On(~hasCard == true) / Enabled(true)
                << On(~hasCard == false) / Enabled(false)
                << Button("1") << Button("2") << Button("3")
                << Button("4") << Button("5") << Button("6")
                << Button("7") << Button("8") << Button("9")
                << Button("*") << Button("0") << (Button("\\#") << []{})
                )
            << (Box("#funboard {flow: grid 1}")
                << On(~hasCard == true) / Enabled(true)
                << On(~hasCard == false) / Enabled(false)
                << (Button("CANCEL") << Background("yellow") << [=](auto e){ psw->erase(psw->stringValue().size() - 1, 1); })
                << (Button("CLEAR") << bg << [=](auto e){ *psw = ""; })
                << (Button("ENTER") << Background("green")
                    << [=](auto e){
                            if(psw->stringValue() == userPsw->stringValue()) {
                                *mesg = "<color=green> Login Successfully";
//                                *mesg = "<color=blue> Select Operation";
                                screen->removeChildren();
                                screen << mesg
                                    << "select operation"
                                    << (HBox("{flow: row; align: stretch end;}")
                                        << (sideTextL << "Deposit" <<"" <<"" <<"")
                                        << Box()
                                        << (sideTextR << "Withdraw" <<"" <<"" <<"" )
                                        );
                                *login = true;
                            }
                            else { ++ wrongCount;
                                *mesg = "<color=red> Wrong Password";
                                *err = "Left " + to_string(4 - *wrongCount) + " Attempts";
                            }
                            *psw = "";
                        }
                    )
                << Button("")
                )
            )
        
    );
    makeInternal();   // disable inspection
}



int main(int argc, const char** argv)
{
  GApp app(argc, argv);
  app.canInspect();

  return app.start(new ATM);
}
