#include "nameToNumber.hpp"

int nameToNumber(char n)
{
  if(n<'a' || 'r'<n)
    return -1;
  return n-'a';
}
