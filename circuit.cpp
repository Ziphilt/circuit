#include <iostream>
#include <vector>
#include <utility>
using namespace std;

struct Node;
struct Gate;
struct And;
struct Or;
struct Not;
void debugNode(pair<string,Node>);

struct Node {
  Node();
  Node(int);
  int newState;
  int oldState;
};

struct Gate {
  virtual void simulate1() = 0;
  virtual void simulate2() = 0;
};

struct Not: public Gate {
  Not();
  Not(Node*, Node*);
  void simulate1();
  void simulate2();
  Node* in;
  Node* out;
};

struct And: public Gate {
  And();
  And(Node*, Node*, Node*);
  void simulate1();
  void simulate2();
  Node* in1;
  Node* in2;
  Node* out;
};

struct Or: public Gate {
  Or();
  Or(Node*, Node*, Node*);
  void simulate1();
  void simulate2();
  Node* in1;
  Node* in2;
  Node* out;
};

int main()
{
  Node set(0);
  Node reset(0);
  Node q(0);
  Node n1(0);
  Node n2(0);
  Not notG(&reset,&n1);
  And andG(&n1,&n2,&q);
  Or orG(&set,&q,&n2);
  vector<Gate*> circuit = {&notG,&andG,&orG};
  vector<pair<string, Node> > nodes; // {pair<string,Node> ("set",set),pair<string,Node> ("reset",reset),pair<string,Node> ("q",q),pair<string,Node> ("n1",n1),pair<string,Node> ("n2",n2)};
  while(1) {
    cout << "state? ";
    if (cin) {
      cin >> set.newState >> reset.newState;
      for(vector<Gate*>::iterator p = circuit.begin(); p!=circuit.end(); ++p){
        (**p).simulate1();
      }
      nodes = {pair<string,Node> ("set",set),pair<string,Node> ("reset",reset),pair<string,Node> ("q",q),pair<string,Node> ("n1",n1),pair<string,Node> ("n2",n2)};
      for(vector<pair<string, Node> >::iterator p = nodes.begin(); p!=nodes.end(); ++p){
        debugNode(*p);
      }
      cout << endl;
      for(vector<Gate*>::iterator p = circuit.begin(); p!=circuit.end(); ++p){
        (**p).simulate2();
      }
    } else {cout << endl; break;}
  }
  return 0;
}



Node::Node(): newState(0), oldState(0) {}
Node::Node(int i): newState(i), oldState(i) {}

void debugNode(pair<string,Node> pr){
  string s = pr.first;
  Node n = pr.second;
  cout << s << ": " << n.oldState << " " << n.newState << endl;
}



Not::Not(): in(0), out(0) {}
Not::Not(Node* inp, Node* outp): in(inp), out(outp) {}
void Not::simulate1(){out->newState = !(in->oldState);}
void Not::simulate2(){in->oldState = in->newState;}



And::And(): in1(0), in2(0), out(0) {}
And::And(Node* in1p, Node* in2p, Node* outp): in1(in1p), in2(in2p), out(outp) {}

void And::simulate1(){
  int output = 0;
  if (in1->oldState && in2->oldState) output = 1;
  out->newState = output;
}

void And::simulate2(){
  in1->oldState = in1->newState;
  in2->oldState = in2->newState;
}



Or::Or(): in1(0), in2(0), out(0) {}
Or::Or(Node* in1p, Node* in2p, Node* outp): in1(in1p), in2(in2p), out(outp) {}

void Or::simulate1(){
  int output = 0;
  if (in1->oldState || in2->oldState) output = 1;
  out->newState = output;
}

void Or::simulate2(){
  in1->oldState = in1->newState;
  in2->oldState = in2->newState;
}
