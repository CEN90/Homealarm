# Homealarm

To use the reduce script after modelling in Afra, go to the Tools folder:
> cd Tools

Then runt the script with the .statespace-file given after running Afra:
> ./convert.sh ../Rebeca/src/alarm_simple.statespace

The resulting files will be inside Rebeca/statespaces folder. To get a graph as a pdf file run the aut2pdf script ont the reduced aut file that convert.sh wrote out:
> ./aut2pdf.sh ../Rebeca/statespaces/alarm_simple_reduced.aut 
