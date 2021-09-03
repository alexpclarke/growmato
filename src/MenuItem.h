class MenuItem {
  protected:
    // MenuItem* parent;
    MenuItem** children;
    String label;
    int num_children = 0;
  public:
    MenuItem(int num, String str) {
      label = str;
      MenuItem* arr[num];
      children = arr;
    };

    void addChild(MenuItem* ptr) {
      // ptr->setParent(this);
      children[num_children] = ptr;
      num_children++;
    }

    String getLabel(void) {return label;}

    // void setParent(MenuItem* ptr) {parent = ptr;};

    virtual void Print(void) = 0;
    virtual void Click(void) = 0;
    virtual void ScrollRight(void) = 0;
    virtual void ScrollLeft(void) = 0;
};