Algortimo: MiniblockSort
           Algoritmo de ordenación basado en Dividir y Conquistar, creando 
           particiones estables de datos. Este algoritmo es una variante de 
           Merge Sort de Von Newman.
           Este algoritmo superando en rendimiento y eficiencia a: Mergesort,
           Mergesort-Optimized, Mergesort-in-Place, Timsort y otras variantes 
           de Quicksort.
           
   Autor: Edgard Bernardo Medina Quispe
          <edgardmedina@yahoo.com>
          Arequipa, Perú. 
          Copyright Nov-2017.
          
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
          
Con este Demo viene incorporado con un pequeño compilador portable, llamado
Tiny C Compiler (TCC) que soporta GNU C99 (ANSI C99 + GNU Basic), con este
compilador se puede ejecutar el progrma de comprobación 'sorting_test.c', que 
comprueba el funcionamiento los tiempos de ejecución de varios algoritmos de 
ordenamiento.

    "TCC es un compilador de C ligero y portable, es muy rápido (más que 
     cualquier compilador de desarrollo profesional), la compilación (creación
     de codigo objeto, el enlazamiento y generación de codigo binario) lo hace 
     completamente en RAM, por lo que no necesita crear un ejecutrable en disco,
     (el ejecutable puede crearse si se le indica mediante opciones en la 
     invocación). Esto lo convierte en compilador extremadamente rápido, 
     comparado con otros compiladores de desarrollo profesional. Al permitir
     compilar y ejecutar en RAM, este se puede comporta como un interprete de 
     comandos (pero no lo es) permitiendo mucha flexiblidad para corroboración
     del código fuente en C."

Se provee un script batch llamado 'tc.cmd' en la carpeta de de trabajo que 
invoca el TCC, para conocer a detalle su sintaxis que este ofrece, escriba 
en la línea de comandos
    
        Drive:\ebmq-MiniBlockSort-Demo> tc /?

        
Para ejecutar el programa de comprobación, debe invocar el comando

        Drive:\ebmq-MiniBlockSort-Demo> tc sorting_test.c

        
Este script compilará el programa en RAM y lo ejecuta inmediatamente , los 
resultados son enviados a consola, pero esta salida se verá de manera caótica, 
por el formato la propia consola. Para enviar los resultados a un archivo 
automáticamente y posteriormente suvisualización use la invocación

        Drive:\ebmq-MiniBlockSort-Demo> tc /file sorting_test.c
o
        Drive:\ebmq-MiniBlockSort-Demo> tc /f sorting_test.c
    
Esta sintaxis enviara la salidas a consola a un archivo autogenerado enumerado
y al finalizar el programa, el script visualizará el archivo autogenerado en 
una ventana independiente.

En caso se desee ver el ranking de los algoritmo y las estadisticas detallas
invoquese el programa com el parametro 'true'.

        Drive:\ebmq-MiniBlockSort-Demo> tc /f sorting_test.c  true
        
Adicionalmente si desea saber cuanto tiempo demora en ejecutar el programa
agrege la opción /time 

        Drive:\ebmq-MiniBlockSort-Demo> tc /time /file sorting_test.c  true
o
        Drive:\ebmq-MiniBlockSort-Demo> tc /t /f sorting_test.c  true

Para saber más sobre las opciones de tc.cmd escriba

        Drive:\ebmq-MiniBlockSort-Demo> tc /?
o
        Drive:\ebmq-MiniBlockSort-Demo> tc -h
