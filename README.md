# SO

## Problem jedzących filozofów

### 1. Opis problemu
Problem jedzących filozofów to przykład zadania synchronizacji procesów, które korzystają z współdzielonych zasobów.
Pięciu filozofów siedzi przy stole. Przed każdym filozofem znajduje się talerz. Pomiędzy każdym talerzem znajduje się widelec. Do zjedzenia posiłku potrzebne są dokładnie dwa widelce. Filozof może myśleć lub jeść. Po skończeniu jedzenia odkłada oba widelce. Rozwiązanie problemu powinno zapewnić, aby żaden filozof nie głodował i aby każdy na zmianę myślał i jadł.

Problemem, który może się pojawić jest zakleszczenie (ang. deadlock), gdzie każdy z filozofów podniesie tylko lewy widelec i będzie czekał na prawy (lub podniesie prawy i będzie czekał na lewy).

Rozwiązania:
- kelner - filozofowie pytają go o pozwolenie na wzięcie widelca
- hierarchia zasobów - np. ponumerowanie widelców i ustalenie kolejności podnoszenia
- rozwiązanie Chandy/Misra

Źródło: https://pl.wikipedia.org/wiki/Problem_ucztuj%C4%85cych_filozof%C3%B3w

### 2. Wątki i co reprezentują

### 3. Sekcje krytyczne i rozwiązanie

### 4. Instrukcja uruchomienia
