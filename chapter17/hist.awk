BEGIN{FS=","; a[0]= 0; a[1] = 0; a[2]=0; a[3]=0;}

{
for ( i = 1; i <= NF; i++ ) { a[$i]++; }

}
END{ print a[0], a[1], a[2], a[3]; }
