class SetTemp : public MenuItem {
  protected:
    
  public:
    String label = "Set Temp"; 
    using MenuItem::MenuItem;

    void Click(void) {
      // currentMenuItem = parent;
    }
    void ScrollRight(void) {
      if (temp < MAX_TEMP) temp++;
    }
    void ScrollLeft(void) {
      if (temp > MIN_TEMP) temp--;
    }

    void Print() {
      lcd->clear();
      lcd->setCursor(0, 0);
      lcd->print("Temperature: ");
      lcd->setCursor(0, 1);
      lcd->print(temp);
    }
};