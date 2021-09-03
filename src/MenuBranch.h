class MenuBranch : public MenuItem {
  protected:
    int index;
    MenuItem** children;
    int num_children;
  public:
    MenuBranch(String label_, int size) {
      label = label_;
      MenuItem* arr[size];
      children = arr;
      index = 0;
      num_children = 0;
    };

    void addChild(MenuItem* ptr) {
      ptr->setParent(this);
      children[num_children] = ptr;
      num_children++;
    };

    void Click(void) {
      currentMenuItem = children[index];
    };

    void ScrollLeft(void) {
      Serial.println("L1");
      index = index == 0 ? num_children - 1 : index + 1;
      currentMenuItem->Print();
    };

    void ScrollRight(void) {
      Serial.println("R1");
      index += (index + 1) % num_children;
      currentMenuItem->Print();
    };

    void Print() {
      // TODO: unable to acceess the children's labels.
      Serial.println((uint32_t)this);
      Serial.println((uint32_t)children[0]);
      Serial.println((uint32_t)&(children[0]->label));
      Serial.println(children[0]->label);

      // lcd->clear();
      // lcd->setCursor(0, 0);
      // lcd->print(">");

      // if (num_children > 0) {
      //   lcd->setCursor(2, 0);
      //   lcd->print(children[0]->getLabel());
      // }

      // if (num_children > 1) {
      //   lcd->setCursor(2, 1);
      //   lcd->print(children[1]->getLabel());
      // }
    };
};