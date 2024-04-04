set xrange [0:20]
set yrange [0:20]
set pointsize 2
unset key
plot 'programa3_10.txt' with points, 'programa3_10.txt' with lines
set terminal pngcairo
set output './dibujos/camino_10.png'
replot
