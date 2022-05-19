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

    auto mesg = Text("Welcom");
    auto hasCard = false;
    auto screen = Box("#screen")
        << mesg;
    
  // main box
  *this
    << QuitDialog("Quit","Changes will be lost", &changed)
    << (VBox(".window")
        << Label(".label ATM ")
        << (HBox()
            << (VBox()
                << (HBox("{border: rounded; display: flex;  flex-direction: column; }")
                  << (VBox("{flex: column; align-items: flex-end;}") << Button("") << Button("") << Button() << Button())
                  << screen
                  << (VBox() << Button("") << Button("") << Button() << Button())
                    )
                << HBox() << Label(".label Cash")
                )
            << (VBox()
                << (Button(".label RECEIPT")
                    << On.click / [=](GMouseEvent* e){ *mesg = "Print RECEIPT"; }
                    )
                << (Button(".label CARD")
//                    << On(~hasCard == true)  / "<c=red><i> Selected"
//                    << On(~hasCard == false) / "<c=blue> Deselected"
                    << On.click / [=](GMouseEvent* e){ *mesg = "Card Inserted"; enable(false);}
                        << Box()
                   )
                )
            )
        << (HBox("#keyboard")
          << (VBox(".nobtn") << Button("1") << Button("4") << Button("7") << Button("*"))
          << (VBox(".nobtn") << Button("2") << Button("5") << Button("8") << Button("0"))
          << (VBox(".nobtn") << Button("3") << Button("6") << Button("9") << Button("#"))
          << (VBox() << Button("CANCEL") << Button("CLEAR") << Button("ENTER") << Button())
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

