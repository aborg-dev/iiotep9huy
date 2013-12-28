#include <iostream>
#include <cstdlib>
#include <cstdio>

class IStream
{
public:
  IStream(std::istream& in) {}

  /*template<typename... Params>
  void read(Params... params);*/

  template<typename T, typename... Params>
  T read(Params... P);

  //template<typename T, typename... Params>
  //T read();
};

template<>
int IStream::read<int> ()
{
  std::cout << "1 param" << std::endl;
  return 0;
}

template<>
int IStream::read<int, int> (int l, int r)
{
  std::cout << l << " " << r << std::endl;
  std::cout << "2 params" << std::endl;
  return 0;
}

int main()
{
  IStream* x = new IStream(std::cin);
  x->read<int, int>(3, 5);
  x->read<int>();
  return 0;
}
