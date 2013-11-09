generowanie_sygnalu
===================

Generowanie sygnału

Generuje sygnal o zadanych parametrach. Dziala na Windows i Linux. Aby wyswietlac sygnaly za pomoca gnuplot nalezy:

1) Na Linuxie (przyklad z Debiana)
  wpisac w konsoli: sudo apt-get install gnuplot gnuplot-x11
  
2) Na Windowsie (przyklad z Windows 7)
  - nalezy zainstalowac gnuplot: http://www.tatsuromatsuoka.com/gnuplot/Eng/winbin/gp470-20120916-win32-mingw-setup.zip
  - przy instalacji wazne jest aby zaznaczyc dodanie programu do zmiennej systemowej PATH
  
Projekt oparty jest o CMake. Aby skompilowac:
1) Na Linuxie
2) Na Windowsie (Windows 7 z QtCreatorem - zestaw narzedzi: MinGW 4.8)
  W QtCreatorze Plik->Otworz plik lub projekt. Nalezy zaznaczy plik Cmake i kliknac zbuduj
