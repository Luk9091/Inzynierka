# Projekt Inżynierski
# Temat: Pojazdy autonomiczne z rozwiązywaniem arbitrażu

Powyższe repozytorium przedstawia *Projekt pracy inżynierskiej* będzie zbudowanie pojazdów autonomicznych, których zadaniem będzie poruszanie się po makiecie miasta.
Znajdowanie optymalnej trasy oraz możliwość rozwiązywania problemów dynamicznie pojawiających się problemów na wyznaczonej trasie.


# Założenia budowy:
Projekt zakłada zbudowanie przynajmniej dwóch pojazdów. 
Każdy z pojazdów powinien spełniać poniższe założenia:
- [ ] Rozpoznawanie obrazu
    - [ ] Rozpoznawanie znaków
        - [ ] 1. Stop
        - [ ] 2. Zakręt
            - [ ] w prawo
            - [ ] w lewo
        - [ ] 3. Skrzyżowanie
        - [ ] 4. Ustąp pierwszeństwa
        - [ ] 5. Masz pierwszeństwo
- [ ] Tworzenie dynamicznej mapy 
    - [ ] Wyznaczanie trasy na mapie
    - [ ] Pomiar odległości
- [ ] Sterowanie silnikami
- [ ] Pomiary redundantne:
    - [ ] Pomiar temperatury
    - [ ] Pomiar azymutu
    - [ ] Pomiar wysokości nad poziomem morza
    - [ ] Pomiar przyspieszenia
    - [ ] Pomiar nachylenia


# Budowanie mapy
Mapa będzie przedstawiać, kwadratowe tile o ustalonych wymiarach `8b x 8b`