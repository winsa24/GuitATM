//
//  Guit GUI Toolkit Examples.
//  Copyright © 2022 Eric Lecolinet. All rights reserved.
//  http://www.telecom-paris.fr/~elc
//

#include <guit.hpp>
#include <gstatechart.hpp>
using namespace guit;     // avoids prefixing Guit classes and functions by guit::

class MyApp : public GWindow {
    

public:
    
  MyApp() : GWindow("#myapp My ATM")
  {
      auto userPsw = Text("13579");


      auto statemac = StateChart();      // creates a state machine

      auto init = SuperState(statemac);  // hierarchical state belonging to 'statemac'
      statemac->setInitial(init);        // 'init' is the initial state of 'statemac'

      auto idle = State(init);           // substate of the 'init' state
      init->setInitial(idle);            // 'idle' is the initial state of 'init'
      auto auth      = State(init);
      auto ops    = State(init);
      auto exit  = State(init);
      auto withdraw    = State(init);
      auto deposit     = State(init);

      auto display = Label("Welcome")
      << On(~idle == true)         / ("Please insert a card")
      << On(~auth == true)    / ("Please enter password")
      << On(~ops == true)  / ("Select operation")
      << On(~exit == true)    / ("Safely Exit")
      << On(~withdraw == true)     / ("Withdraw")
      << On(~deposit == true)  / ("Deposit");

      auto mesg = Label("Welcome");
      auto psw = Text();
      auto err = Text();
      auto sideTextL = VBox();
      auto sideTextR = VBox();
      
      auto screen = Box("#screen {flow: column; align: stretch stretch;}")
          << display
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
      };
      auto confirmBtn = [=](auto e){
          *en = true;
          sideTextL->removeChildren();
          sideTextR->removeChildren();
          sideTextL << "Exit";
          sideTextR << "Continue";
      };
    //auto depositMoney = [=](auto e){    //elc: (auto e) not needed
      auto depositMoney = [=]{
          *mesg = "deposit";
          cashBox << bg << bd << interactMoney;
          sideTextL->removeChildren();
          sideTextR->removeChildren();
          sideTextL << "Cancel";
          sideTextR << "Confirm";
      };
    //auto withdrawMoney = [=](auto e){
      auto withdrawMoney = [=]{
          screen->remove(psw);
          *mesg = "Select the amount of money";
          sideTextL->removeChildren();
          sideTextR->removeChildren();
          sideTextL << "10" << "20" << "50" << "Cancel";
          sideTextR << "100" << "300" << "Others" << "";
      };
      
      
     *this
//      << QuitDialog("Quit","Changes will be lost", &changed)
      << (VBox(".window {align: stretch stretch;}")
          << Guard(~wrongCount > 3)  / (auth >> idle)
          << Label(".label ATM ")
          << (HBox()
              << (VBox()
                  << (HBox("{align: stretch end;}")
                      << (VBox()
                          << Button("") << Button() << Button()
                          // << (Button("{#deposit}") << (ops >> deposit) << depositMoney)
                          // elc: / means sequence => depositMoney called only if transition fired
                          << (Button("#deposit") << (ops >> deposit) / depositMoney)
                          )
                      << screen
                      << (VBox()
                          << Button("") << Button("") << Button()
                          // << (Button("{#withdraw}") << (ops >> withdraw) << withdrawMoney)
                          << (Button("#withdraw") << (ops >> withdraw) / withdrawMoney)
                          )
                      )
                  << (VBox()
                      << Label(".label Cash")
                      << cashBox
                      )
                  )
              << (VBox()
                  << (Button(".label RECEIPT")
                      << en
                      // << On.click / [=](GMouseEvent* e){ *mesg = "Print RECEIPT"; }
                      << On.click / [=]{ *mesg = "Print RECEIPT"; }  // elc: GMouseEvent not needed
                      )
                  << (Button(".label CARD")
                      << (idle >> auth)  // is working
                      << On.click
                      / [=]{ *mesg = "<color=green> Card Inserted"; }
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
                  // << Guard(~auth == true)   / Enabled(true)
                  // << Guard(~idle == true)   / Enabled(false)  // not working
                  // Guard => like a if when the button is clicked
                  // On => activated when the value of auth or idle changes
                  << On(~auth == true)   / Enabled(true)  //elc
                  << On(~idle == true)   / Enabled(false) //elc
                  << [=](auto e){ if(e->child()->textValue() != "*" && e->child()->textValue() != "\\#" )*psw += e->child()->textValue();}
                  << Button("1") << Button("2") << Button("3")
                  << Button("4") << Button("5") << Button("6")
                  << Button("7") << Button("8") << Button("9")
                  << Button("*") << Button("0") << (Button("\\#") << []{})
                  )
              << (Box("#funboard {flow: grid 1}")
                  << (Button("CANCEL") << Background("yellow") << [=](auto e){ psw->erase(psw->stringValue().size() - 1, 1); })
                  << (Button("CLEAR") << bg << [=](auto e){ *psw = ""; })
                  << (Button("ENTER") << Background("green")
    /*
                      // << Guard(~psw == ~userPsw) / (auth >> ops) // not working // elc: it works, callback is fired
                      << Guard(~psw == ~userPsw) / (auth >> ops) / []{cerr << "it works!" << endl;}
                      << [=](auto e){
                              if(psw->stringValue() == userPsw->stringValue()) {
                                  *mesg = "<color=green> Login Successfully";
                              }
                              else { ++ wrongCount;
  //                                *mesg = "<color=red> Wrong Password";
                                  *err = "Left " + to_string(4 - *wrongCount) + " Attempts";
                              }
                              *psw = "";
                          }
  */
                      // equivalent but simpler:
                      << Guard(~psw == ~userPsw)
                      / (auth >> ops)
                      / [=]{*mesg = "<color=green> Login Successfully"; *psw = "";}

                      << Guard(~psw != ~userPsw)
                      / [=]{++ wrongCount; *err = "Left " + std::to_string(4 - *wrongCount) + " Attempts"; *psw = "";}
                      )
                  << Button("")
                  )
              )
      );
      // starts the state machine
      statemac->start();
  }

};

int main(int argc, const char** argv) {
  GApp app(argc, argv);  // the Application Context MUST be created first!
  app.canInspect();      // allows inspecting the gadgets (optional)

  app
  << ".mesg {font: bold 36 arial,sans-serif}"   // CSS style
  << "@import myapp.css;";      // CSS file

  return app.start(new MyApp);
}

