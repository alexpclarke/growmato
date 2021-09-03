class MenuItem {
  protected:
    MenuItem* parent;
  public:
    String label;

    MenuItem() {
      label = "Default Label";
    };

    MenuItem(String label_) {
      label = label_;
    };

    String getLabel(void) {
      // Serial.println("L0");
      // Serial.println((uint32_t)this);
      // Serial.println((uint32_t)&label);

      return label;
    };

    void setParent(MenuItem* parent_) {
      parent = parent_;
    };

    virtual void ScrollLeft(void) = 0;
    virtual void ScrollRight(void) = 0;
    virtual void Click(void) = 0;
    virtual void Print(void) = 0;
};