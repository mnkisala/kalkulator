#include <string_view>
#include <array>
#include <cmath>
#include <stdexcept>
#include <functional>
#include <iostream>

struct Operator
{
  /* znaczek ktory go reprezentuje */
  char reprezentacja;
   
  /* kolejnosc wykonywania dzialan (przyp. red.) */
  uint32_t priorytet;
   
  /* dzialanie ktore wykonuje */
  std::function<double(double, double)> operacja;
};

/* lista dostepnych operatorow */
const std::array OPERATORY
{
  Operator{'+', 1, [](auto a, auto b){return a + b;}},
  Operator{'-', 2, [](auto a, auto b){return a - b;}},
  Operator{'*', 3, [](auto a, auto b){return a * b;}},
  Operator{'/', 3, [](auto a, auto b){return a / b;}},
  Operator{'^', 4, [](auto a, auto b){return std::pow(a, b);}},
};

bool czy_jest_operatorem(char reprezentacja)
{
  /* przelatujemy wszystkie operatory w poszukiwaniu tego
   * ktory ma taka reprezentacje */
  for(auto op : OPERATORY)
    if(reprezentacja == op.reprezentacja) return true; /* return przerywa funkcje */
   
  /* jak nie ma to nie ma */
  return false;
}

Operator uzyskaj_operator(char reprezentacja)
{
  /* przelatujemy wszystkie operatory w poszukiwaniu tego
   * ktory ma taka reprezentacje */
  for(auto op : OPERATORY)
    if(reprezentacja == op.reprezentacja) return op; /* return przerywa funkcje */
   
  /* jesli nic nie znalezlismy to programista cos
   *  S P I E T R U S Z Y L
   * i nie zna sie na swojej pracy */
  throw std::runtime_error("nieznany operator");
}

size_t pozycja_operatora_o_najnizszym_priorytecie(std::string wyrazenie)
{
  /* zaczynamy od zara */
  size_t pozycja_operatora = 0;
   
  /* szukamy operatorow w wyrazeniu*/
  for(size_t i = 0; i < wyrazenie.length(); i++)
  {
    if(czy_jest_operatorem(wyrazenie[i]))
    { 
      if(pozycja_operatora == 0)
        pozycja_operatora = i;
      else if(uzyskaj_operator(wyrazenie[i]).priorytet < uzyskaj_operator(wyrazenie[pozycja_operatora]).priorytet)
        pozycja_operatora = i;
    }
  }

  return pozycja_operatora;
}

double oblicz(std::string wyrazenie)
{
  size_t pozycja_operatora = pozycja_operatora_o_najnizszym_priorytecie(wyrazenie);
   
  /* jesli jest operator to dzielimy rownanie na pol i obiczamy
   * mniejsze polowki */
  if(pozycja_operatora != 0)
  {
      auto a = wyrazenie.substr(0, pozycja_operatora); /* polowka nr. a */
      auto b = wyrazenie.substr(pozycja_operatora+1); /* polowka nr. b */
      auto operacja = uzyskaj_operator(wyrazenie[pozycja_operatora]).operacja; /* funkcja ktora reprezentuje operatator */
       
      return operacja(oblicz(a), oblicz(b));
  }
  /* jesli nie ma operatara to przeksztalcamy napis na liczbe */
  else return std::stod(wyrazenie); /* std::stod - string to double [potrzeba zalacznika] */ 
}

 
 
int main(int argc, char** argv)
{
  /* jesli uzytkownik nie poda argumetnow to zapytaj o rownanie */
  if(argc < 2)
  {
    std::string rownanie;
    std::cout << "wprowadz rownanie matematyczne:\n" 
              << ">>> ";
    std::cin >> rownanie;

    std::cout << " = " << oblicz(rownanie) << '\n';
  }
  /* jesli jest przynajmniej jeden argument to go oblicz */
  else 
  {
    std::cout << oblicz(argv[1]) << '\n';
  }
}
