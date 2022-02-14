# Kliensoldali webprogramozás csoport ZH

## Nyersanyag

Letöltés és kicsomagolás után ebbe dolgozzatok. A feladatok külön mappákba vannak elkülönítve. Ezt a csomagot letömörítve töltsétek fel majd Canvasre. **Mindenki figyeljen arra, hogy jó csomagot töltsön fel!**

https://github.com/anonymus1928/Kliensoldali/tree/master/ZH/2.%20csoport/csoportzh.zip

## Feladatok

### 1. Covid megyénkénti eloszlás (9 pont)

A [Chart.js](https://www.chartjs.org/) javascript könyvtár segítségével készíts egy adathalmazt tartalmazó tortadiagramot! A szükséges adatokat a ```1. feladat/index.js``` file ```data``` konstans változója tartalmazza.

Példa a ```data``` tartalmára:
```json
{
    // ...
    "Budapest": {
        "color": "rgba(106,102,213,0.8)",
        "covid": "82044"
    },
    // ...
}
```

1. [JSDelivr](https://www.chartjs.org/docs/latest/getting-started/)-en keress rá a ```chart.js```-re (fejlesztő: chartjs) és az alapértelmezett csomagjának hivatkozását illeszd be az ```1. feladat/index.html``` file végére. Ügyelj arra, hogy az ```index.js``` mindenképpen a most beillesztett könyvtár **után** legyen! *(1 pont)*

2. Készíts két tömböt (például: ```covid```, ```color```) és töltsd fel őket a ```data```-ban lévő ```object```-ek azonos nevű adattagjaival. Figyelj arra, hogy a ```covid``` tömbbe már számként kerüljenek be! (```parseInt()```) Ezt követően írd ki a két tömböt a ```console```-ra! *(3 pont)*
  - Tipp: Használd az ```Object.values(data)``` metódust!

3. Készítsd el a diagramot a következő lépések mentén! *(5 pont)*<br>Minta: https://www.chartjs.org/docs/latest/getting-started/<br>Tortadiagram: https://www.chartjs.org/docs/latest/charts/doughnut.html
  - A DOM-ból emeld ki a ```#covid``` azonosítójú ```canvas``` elemet!
  - Példányosítsd a ```Chart``` osztályt a ```new Chart(canvas, options)``` konstruktorral!
  - Az ```options```-ben a következő beállításokat végezd el!
    - A diagram típusa legyen ```pie```!
    - Az x-tengely feliratai legyenek a ```data``` kulcsai (a megyenevek)
      - Tipp: az ```Object``` osztály melyik metódusa lehet itt a segítségünkre?
    - A napi legmagasabb értékek beállítása a következők legyenek:
      - Az adatokat a ```covid``` változó tartalmazza
      - A körcikkek színe (```bachgroundColor```) legyen a ```color``` változó tartalma



### 2. Színváltó doboz (9 pont)

Adott egy képernyő méretű ```div``` elem, amelynek az alsó és felső margója megegyezik a ```viewport``` magasságával. A feladatod, hogy felokosítsd az alábbiak szerint ezt a dobozt. A ```div``` háttérszínének áttetszősége (nem a ```div``` ```opacity``` értéke!), a ```viewport```-ba érve, a ```viewport``` közepéhez viszonyítva változzon az ```rgba(0, 56, 68, 0)``` és ```rgba(0, 56, 68, 1)``` között. Mindezt aszerint, hogy a ```viewport``` közepe a ```div``` hányad részénél van. Ehhez segítségül a képernyő közepén van egy rögzített szürke csík (a feladat megoldásához nem kell használni).

1. Készíts egy függvényt (```getThreshold```), ami bemenetként vár egy egész számot (```n```) és visszaad egy tömböt, ami ```n + 1``` db, [0,1] közötti értékeket tartalmaz egyenletes eloszlással. *(2 pont)*
  - például: ```getThreshold(5) === [0, 0.2, 0.4, 0.6, 0.8, 1]```

2. Készíts egy ```IntersectionObserver```-t és hívd meg a ```.square``` stílusosztályú elemre! A konstruktorban a ```threshold```-nál hívd meg a ```getThreshold``` függvényt egy tetszőleges paraméterrel (például: 20; figyelj arra, hogy minél nagyobb számot adsz meg, annál többször fog lefutni a konstruktorban átadott callback függvény)! *(2 pont)*

3. Az observer konstruktorában átadott függvényben számítsd ki, hogy a ```viewport``` közepe a ```div```-et "hányad" részénél metszi. *(4 pont)*
  - Tipp: Az ```IntersectionObserverEntry```-nek a ```boundClientRect```-en belüli adatokra lesz szükséged, ez azonos a ```getBoundingClientRect()``` algoritmusa által generáltakkal.
  - Tipp: A ```viewport``` magasságát a ```window.innerHeight``` adattagon keresztül éred el.

4. Állítsd be a kiszámított érték alapján a ```div``` háttérszínét! *(1 pont)*
  - Tipp: ````rgba(0, 56, 68, ${ratio})````




### 3. Telefonkönyv (12 pont)

Progresszív fejlesztéssel okosítsd fel a megadott űrlapot! 2 féle telefonszám (mobil, vezetékes) megadása lehetséges, 2 különböző input mezőben. A feladatod, hogy szétválaszd az előhívószámot és körzetszámot a többi számjegytől, hogy a következő formátumban lehessen megadni:

```0620``` / ```1234567``` vagy ```061``` / ```1234567```

A felokosítást követően 2 gombbal lehessen hozzáadni ezeket a mezőket, illetve legyen lehetőség őket eltávolítani.

**Nincs előre beégetett adat!**

**A beírt adatokat nem kell validálni (most kivételesen)!**

**A form submit eseményével most nem kell foglalkozni!**

*Ha shadow dom-ot használsz, a bootstrap működéséhez másold be ezt a metódust a class-ba és hívd meg a konstruktorból.*

```javascript
getBootstrap() {
    const style = document.createElement('style');
    style.innerHTML = '@import url("https://cdn.jsdelivr.net/npm/bootstrap@5.0.0-beta2/dist/css/bootstrap.min.css");';
    this.shadowRoot.appendChild(style);
}
```


1. Generálj két gombot, "Mobil" és "Vezetékes" feliratokkal. *(2 pont)*
  - Add hozzájuk a ```btn```, ```btn-primary``` és ```m-2``` stílusosztályokat (```class```)!
  - Az ```id```-jaikat állítsd be "Mobil"-ra vagy "Vezetékes"-re (ez a későbbiekben jól fog jönni)
  - Adj hozzájuk ```eventListener```-t ```click``` eseményre! Kattintás után állítsd ```disabled```-re és hívd meg a metódust, amivel kigenerálod a megfelelő beviteli csoportot!

2. Készíts egy metódust, amivel az űrlaphoz adsz egy beviteli csoportot (ld.: ```template```)! *(4 pont)*
  - A ```h5``` címsort állítsd be annak megfelelően, hogy a ```Mobil```-ra vagy a ```Vezetékes```-re kattintottunk!
  - A ```select```-et töltsd fel az alábbi minta előhívószámokkal/körzetszámokkal!

```javascript
const options = {
    mobile: ['0620', '0630', '0650', '0660', '0670'],
    landline: ['061', '0622', '0623', '0674', '0675']
}
```

  - A ```button```-höz adj ```eventListener```-t click eseményre! Töröld a teljes beviteli csoportot! Az 1. pontban legenerált gombok közül a megfelelő ```disabled``` adattagját állítsd ```false```-ra!
    - Tipp: A teljes sor törléséhez használd a ```closest('.row')``` metódust!

3. Használd az index.html-ben lévő template-et! (2 pont)

4. Használj shadow dom-ot! (2 pont)

5. Zárdd egységbe a megoldást! (2pont)
  - ```customElements``` VAGY ```new myBeautifulClass(...)```