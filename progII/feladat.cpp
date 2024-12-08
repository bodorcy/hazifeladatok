#include <iostream>
#include <string>
#include <set>
#include <memory>
#include <algorithm>
#include <vector>

using namespace std;
class Konyvtar;
class FRElem {
protected:
    string nev;
    Konyvtar * szulo;

    FRElem(const string& nev, Konyvtar* szulo) :
            nev(nev), szulo(szulo) { }
    FRElem(const FRElem& fre) : nev(fre.getNev()), szulo(fre.szulo) {};
public:
    virtual const string& getNev() const {
      return nev;
    }
    virtual const Konyvtar* getSzulo() const noexcept {
      return szulo;
    }
    virtual void setSzulo(Konyvtar * szulo){
      this->szulo = szulo;
    }
    virtual string getTeljesUtvonal() const{
      if(!szulo)
        return getNev();
      return reinterpret_cast<FRElem *>(szulo)->getTeljesUtvonal() + getNev() + "/";
    };
    virtual FRElem * clone() const{
      return new FRElem(*this);
    }
    virtual void rm(){
      delete this;
    }
    class Compare{
    public:
        bool operator()(const FRElem* egyik, const FRElem* masik) const{
          return egyik->getNev() < masik->getNev();
        }
    };
    virtual ~FRElem() = default;
};
class Fajl : public FRElem{
    string tartalom;
public:
    Fajl(const string& nev, Konyvtar* szulo) : FRElem(nev, szulo){}
    Fajl(const Fajl& f) : FRElem(f.getNev(), f.szulo), tartalom(f.getTartalom()){};

    const string &getTartalom() const {
      return tartalom;
    }
    void setTartalom(const string &tartalom) {
      this->tartalom = tartalom;
    }
    void rm(){
      delete this;
    }
    Fajl * clone() const override{
      return new Fajl(*this);
    }
    Fajl& operator=(const Fajl& masik) {
      if(this == &masik)
        return *this;

      this->nev = masik.nev;
      this->szulo = masik.szulo;
      this->tartalom = masik.tartalom;

      return *this;
    }
};
class Konyvtar : public FRElem{
private:
    Konyvtar(const string& nev, Konyvtar * szulo) : FRElem(nev, szulo) {}
    Konyvtar(const Konyvtar& k) : FRElem(k.getNev(), k.szulo){
      for(auto fre : k.gyerekek){
        Konyvtar * fre_is_konyvtar = dynamic_cast<Konyvtar *>(fre);
        if(!fre_is_konyvtar)
          this->gyerekek.insert(dynamic_cast<Fajl *>(fre)->clone());
        else{
          this->gyerekek.insert(fre_is_konyvtar->clone());
        }
      }
    }
public:
    set<FRElem *, Compare> gyerekek;
    static Konyvtar GYOKER;

    Konyvtar& operator/(const string& nev){
      Konyvtar * k = new Konyvtar(nev, this);

      if(gyerekek.find(k) != gyerekek.end()) {
        delete k;
        throw exception();
      }
      gyerekek.insert(k);

      return *k;
    }
    Fajl& operator%(const string& nev){
      Fajl * f = new Fajl(nev, this);

      if(gyerekek.find(f) != gyerekek.end()){
        delete f;
        throw exception();
      }
      gyerekek.insert(f);

      return *f;
    }
    Konyvtar& operator>>(const string& nev) {
      if(nev == "..")
        return *(this->szulo ? this->szulo : this);

      auto it = find_if(gyerekek.begin(), gyerekek.end(), [&](FRElem * fre) {return fre->getNev() == nev && dynamic_cast<Konyvtar*>(fre);});

      if(it == gyerekek.end())
        throw exception();

      return *dynamic_cast<Konyvtar*>(*it);
    }
    static Konyvtar* getGyokerPtr() {
      return &GYOKER;
    }
    void ls() const{
      static unsigned level = 0;
      for(unsigned i = 0; i < level; ++i)
        cout << "  ";

      cout << getNev() << endl;
      for(auto fre : gyerekek){
        ++level;
        auto fre_is_konyvtar = dynamic_cast<Konyvtar *>(fre);
        if(!fre_is_konyvtar || fre_is_konyvtar->gyerekek.empty()){
          for(unsigned i = 0; i < level; ++i)
            cout << "  ";
          cout << fre->getNev() << (fre_is_konyvtar ? "" : " (fajl)") << endl;
        }
        else
          fre_is_konyvtar->ls();
        --level;
      }
    }
    void cp(FRElem& file){
      if(gyerekek.find(&file) != gyerekek.end())
        throw exception();

      Konyvtar * file_is_konyvtar = dynamic_cast<Konyvtar *>(&file);
      if(!file_is_konyvtar) {
        Fajl * cpf = dynamic_cast<Fajl *>(&file)->clone();
        cpf->setSzulo(this);
        gyerekek.insert(dynamic_cast<Fajl *>(&file)->clone());
      }
      else {
        file_is_konyvtar->setSzulo(this);
        gyerekek.insert(file_is_konyvtar->clone());
      }
    }
    Konyvtar * clone() const override{
      return new Konyvtar(*this);
    }
    static Konyvtar& getGyoker() {
      return GYOKER;
    }
    void rm() {
      if(this->getNev() == "/"){
        for(auto it = gyerekek.begin(); it != gyerekek.end();) {
          FRElem * fre = *it;
          it = gyerekek.erase(it);
          delete fre;
        }
      }
      else {
        this->szulo->gyerekek.erase(this);
        delete this;
      }
    }

    ~Konyvtar() override{
      for(FRElem * fre : gyerekek)
        delete fre;
      gyerekek.clear();
    }
};
Konyvtar Konyvtar::GYOKER = Konyvtar("/", nullptr);


#ifndef TEST_BIRO

int main() {

  Konyvtar& gyoker = Konyvtar::getGyoker();
  Konyvtar& k2 = gyoker / "elso" / "masodik";
  Konyvtar& k4 = k2 / "harmadik" / "negyedik";
  gyoker / "elso 2";
  gyoker / "elso 3";

  k2 % "readme.txt";
  k4 % "feladat.cpp";
  k4 / "otodik";

  //cout << "Teljes utvonal" << endl;
  //cout << k2.getTeljesUtvonal() << endl;
  //cout << gyoker.getTeljesUtvonal() << endl;
  //  Teljes utvonal
  //  /elso/masodik/
  //  /

  //cout << endl << "ls I." << endl;
  //k2.ls();
  //  ls I.
  //  masodik
  //    harmadik
  //      negyedik
  //        feladat.cpp (fajl)
  //        otodik
  //    readme.txt (fajl)

  //cout << endl << "ls II." << endl;
  //gyoker.ls();
  //  ls II.
  //  /
  //    elso
  //      masodik
  //        harmadik
  //          negyedik
  //            feladat.cpp (fajl)
  //            otodik
  //        readme.txt (fajl)
  //    elso 2
  //    elso 3

  //cout << endl << "operator >>" << endl;
  //Konyvtar& masodik = gyoker >> "elso" >> "masodik";
  //cout << masodik.getNev() << endl;
  //  operator >>
  //  masodik

  cout << endl << "cp" << endl;
  gyoker.cp(k2);
  gyoker.ls();
  //  cp
  //  /
  //    elso
  //      masodik
  //        harmadik
  //          negyedik
  //            feladat.cpp (fajl)
  //            otodik
  //        readme.txt (fajl)
  //    elso 2
  //    elso 3
  //    masodik
  //      harmadik
  //        negyedik
  //          feladat.cpp (fajl)
  //          otodik
  //      readme.txt (fajl)


  cout << endl << "rm I." << endl;
  gyoker.ls();
  (gyoker >> "masodik").rm();
  gyoker.ls();
  //  rm I.
  //  /
  //    elso
  //      masodik
  //        harmadik
  //          negyedik
  //            feladat.cpp (fajl)
  //            otodik
  //        readme.txt (fajl)
  //    elso 2
  //    elso 3

  cout << endl << "rm II." << endl;
  gyoker.rm();
  gyoker.ls();
  //  rm II.
  //  /
}
#endif