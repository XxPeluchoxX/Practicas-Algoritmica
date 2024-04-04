set xrange [0:200]
set yrange [0:200]
set pointsize 2
unset key
plot 'programa3_100.txt' with points, 'programa3_100.txt' with lines
set terminal pngcairo
set output './dibujos/camino_100.png'
replot
