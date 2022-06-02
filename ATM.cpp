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

    auto userPsw = "13579";
    
    auto mesg = Label("Welcom");
    auto psw = Text();
//    char* psw = "";
    auto hasCard = Bool(false);
    auto screen = Box("#screen")
        << mesg
        << psw;
    
    auto bg = Background("pink");
    auto bd = Border("rounded");
    
    auto en = Enabled(true);
    
    auto wrongCount = Int(0);
    
  // main box
  *this
    << QuitDialog("Quit","Changes will be lost", &changed)
    << (VBox(".window")
//        << On(~hasCard == false)  / (*en = true)
        << On(~wrongCount > 3)  / (*en = true)
        << Label(".label ATM ")
        << (HBox()
            << (VBox()
                << (HBox("{align: stretch end;}")
                    << (VBox() << Button("") << Button("") << Button() << Button())
                    << screen
                    << (VBox() << Button("") << Button("") << Button() << Button())
                    )
                << HBox() << Label(".label Cash")
                )
            << (VBox()
                << (Button(".label RECEIPT")
                    << On.click / [=](GMouseEvent* e){ *mesg = "Print RECEIPT"; }
                    )
                << (Button(".label CARD") << en
                    << On(~hasCard == true)  / (*en = false)
                    << On(~hasCard == false) / (*en = true)
                    << On.click
                    / [=](GMouseEvent* e){ *hasCard = true;}
                    / [=]{ *mesg = "<color=green> Card Inserted"; }

                    // waits for 1 second whitout blocking the event loop
                    / SleepFor(2000)

                    // done once the delay is over
                    / [=]{ *mesg = "<color=blue> Enter Password"; }
                        << Box()
                   )
                )
            )
//        << (HBox("#keyboard")
//          << (VBox(".nobtn") << Button("1") << Button("4") << Button("7") << Button("*"))
//          << (VBox(".nobtn") << Button("2") << Button("5") << Button("8") << Button("0"))
//          << (VBox(".nobtn") << Button("3") << Button("6") << Button("9") << Button("#"))
//          << (VBox() << Button("CANCEL") << Button("CLEAR") << Button("ENTER") << Button())
//        )
        << (HBox("#keyboard {align: stretch stretch;}")
            << (Box("#noboard {flow: grid 3}")
                << [=](auto e){ *psw += e->child()->textValue();}
                << Button("1") << Button("2") << Button("3")
                << Button("4") << Button("5") << Button("6")
                << Button("7") << Button("8") << Button("9")
                << Button("*") << Button("0") << Button("#")
                )
            << (Box("#funboard {flow: grid 1}")
                << (Button("CANCEL") << Background("yellow") << [=](auto e){ psw->erase(psw->stringValue().size() - 1, 1); })
                << (Button("CLEAR") << bg << [=](auto e){ *psw = ""; })
                << (Button("ENTER") << Background("green")
                    << [=](auto e){
                            if(psw->stringValue() == userPsw) { *mesg = "<color=green> Login Successfully"; }
                            else { ++ wrongCount;
                                *mesg = "<color=red> Wrong Password";
                                screen << "Left " + (4 - ~wrongCount) + " Attempts";
                            }
                        })
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

