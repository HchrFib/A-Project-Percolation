Projecte-A

Projecte algorítmia procés de "Percolació en grafs aleatoris" Aquest projecte implementa una simulació per treballar amb grafs aleatoris i genera visualitzacions en PDF a partir d'arxius .dot. A més, utilitza una biblioteca anomenada PbPlots per donar suport a les gràfiques.
Requisits

Abans de començar, assegura't de tenir instal·lades les següents eines:

G++: El compilador utilitzat per compilar els fitxers font.
Make: Per gestionar el procés de compilació.
Libxlsxwriter: A C library for creating Excel XLSX files.
INSTALL: http://libxlsxwriter.github.io/getting_started.html

Compilació

Per compilar el projecte i generar l'executable, simplement executa el següent comandament al terminal dins del directori arrel del projecte:

make

Això compilarà els arxius font i els arxius de la biblioteca, i generarà un arxiu executable anomenat "main".
Generació de PDFs

Un cop hagis compilat el projecte, pots generar els arxius PDF a partir dels fitxers .dot executant:

make pdfs

Això convertirà tots els fitxers .dot de la carpeta square_grid_dot en arxius PDF i els guardarà a la carpeta draw_grid_graphs.
Neteja del Projecte

Per netejar els fitxers generats (executables, arxius objecte, fitxers .dot i PDFs), executa:

make clean

Ús del Projecte

Un cop compilat el projecte, pots executar l'arxiu main des del terminal de la següent manera:

./main <model_graph> <graph_size> <type_percolation> [generation_probability]

    <model_graph> pot prendre un dels valors següents:
        s (Square grid)
        g (Geometric)
        b (Binomial Erdos-Renyi)

    <graph_size> indica la quantitat de nodes que tindra el graf

    <type_percolation> pot prendre un dels valors següents:
        s (Site percolation)
        b (Bond percolation)

(Els paramatres es poden posar tant en majúscules com en minúscules)

    [generation_probability] defineix la probabilitat de generació d'arestes, que ha de prendre valor p ∈ [0, 1]. (Parametre opcional, si no s'especifica per defecte es 0)

