

int main(void){
  String s {"Hello World"};
  if (s != "Hello World"){

  }

  String::iterator p = s.begin();
  *p = 'J';

  String::iterator const p = s.begin();
  ++p; //does not work. Iterator is constant. Not pointer to immutable data
  *p = 'J'; // same as above

  /* Solution to const iterator problem. We want a pointer to immutable data */
  String::const_iterator constp = (String const&) s).begin();
  const char letter = *constp; //dereference retrurns a const; makes data read-only

  char s2[] = "Hello World"; //valid

  char* p2 = &s[0]; //valid
  char const* const_p2 = &s[0]; //pointer is a const. data is not const/immutable

}


template <typename Character> class iterator_helper{
  private:
    private Character* ptr;
    using Same = iterator_helper<Character>;

#include <utility> // rel_ops is the namespace that contains the templates
    bool operator==(Same const& rhs)const{
      return this->ptr == rhs.ptr;
    }
    /* != operator automatically created by a template function by just calling
    * !(a == b)
    * if you provide one, then compiler chooses yours over the templated one
    */
    friend String;

  public:
    using value_type = Character;
    using iterator_category = std::random_access_iterator_tag;
    /* or you can inherit from a helper class that has a typedefs for everything
    * class iterator_helper : public std::iterator<std::random_access_iterator_tag, Character>{...}
    */

    /* When an iterator becomes invalid, result in undefined behavior
    * An iterator becomes invalid when the data structure is modified
    * Changing a value is fine but changing positions is modifying the data structure
    * Use version numbers in iterators and data structures to detect differences between the iterator and the data structure
    * Convention in C++ is undefined while Java raises an error
    */

    iterator begin(void){
      iterator p;
      p.ptr = data;
      return p;
    }

    iterator end(void){
      iterator p;
      p.ptr = (char*) storage; //Note: that Chase stored string backwards
      return p;
    }

    const_iterator begin(void) const{
      const_iterator p;
      p = ptr.data;
      return p;
    }

    const_iterator end(void) const{
      const_iterator p;
      p.ptr = (char*) storage;
      return p;
    }

}

/* For project 1c
* 1. write *
* 2. write ++, --
* 3. write begin() and end() functions
*/

/* If you do
* template <typename T>
* class less{ ... }
* then you must call less<int> p;
*/


class less{
public:
  template <typename T>
  bool operator()(T const& x, T const& y){
    return x < y;
  }
}

int main(void){
  less p;

  if(p(5, 10)){
    cout << "hello" << endl;
  }

  if(p(2.5, 3.0)){
    cout << "world!" << endl;
  }

  if(p(3.0, 2)){  // won't compile
    
  }


}
