

class ThingHelperInterface {
public:
  virtual void print(std::ostream&) const = 0;
  virtual void cleanup(void) = 0;
};

template<typename T>
class ThingHelper : public ThingHelperInterface {
private:
  T obj;
public:

};

class Thing {
  void* ptr;
public:
/*  template <typename T>
  Thing& operator=(const T& obj) { // can assign anything to Thing
    ptr = new T{ obj };
    return *this;
  }*/

  template <typename T>
  Thing& operator=(T const& obj) {
    ptr = new ThingHelper<T>{obj};
    return *this;
  }

  void print(std::ostream& out) {
    out << *ptr << endl;
  }
}

// expression templates
// if then else
// original MKS example, ints as arguments
// ChooseType
// mix of static/dynamic polymorphism, when is it available, when to make that choice
