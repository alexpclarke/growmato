class MainMenu : public MenuItem {
  protected:
    int index = 0;
  public:
    using MenuItem::MenuItem;

    void Click(void) {
      currentMenuItem = children[index];
    }
    void ScrollRight(void) {
      Serial.println("scroll right");
      index += (index + 1) % num_children;
    }
    void ScrollLeft(void) {
      Serial.println("scroll left");
      index = index == 0 ? num_children - 1 : index + 1;
    }

    void Print() {
      lcd->clear();
      lcd->setCursor(0, 0);
      lcd->print(">");
      if (num_children > 0) {
        lcd->setCursor(2, 0);
        lcd->print(children[index]->getLabel());
      }
      if (num_children > 1) {
        lcd->setCursor(2, 1);
        lcd->print(children[(index + 1) % num_children]->getLabel());
      }
    }
};