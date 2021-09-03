class MenuLeaf : public MenuItem {
  protected:
    void (*PrintFunction)(void);
    void (*ScrollLeftFunction)(void);
    void (*ScrollRightFunction)(void);
    void (*ClickFunction)(void);
  public:
    MenuLeaf(String label_, void (*PrintFunction_)(void), 
    void (*ScrollLeftFunction_)(void), void (*ScrollRightFunction_)(void),
    void (*ClickFunction_)(void)) {
      label = label_;
      PrintFunction = PrintFunction_;
      ScrollLeftFunction = ScrollLeftFunction_;
      ScrollRightFunction = ScrollRightFunction_;
      ClickFunction = ClickFunction_;
    }

    void Print() {
      PrintFunction();
    }
    void Click(void) {
      ClickFunction();
    }
    void ScrollRight(void) {
      ScrollRightFunction();
    }
    void ScrollLeft(void) {
      ScrollLeftFunction();
    }
};