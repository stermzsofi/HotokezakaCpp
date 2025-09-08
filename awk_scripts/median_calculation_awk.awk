/^#/{
print $0}
!/^#/{
	n = 0
  	# kiszedjük a 3–102. oszlopokat és közben elmentjük az eredeti sorrendet is
  	for (i = 3; i <= NF; i++) {
    		a[n] = $i     # rendezéshez
    		orig[n] = $i  # kiíráshoz eredeti sorrendben
    		n++
  	}

  	# bubble sort a mediánhoz (hatékonyabb algoritmus nem kell 100 elemhez)
  	for (i = 0; i < n; i++) {
    		for (j = i + 1; j < n; j++) {
      			if (a[i] > a[j]) {
        			tmp = a[i]; a[i] = a[j]; a[j] = tmp
      			}
    		}
  	}

  	# medián számítás
  	if (n % 2 == 1) {
    		median = a[int(n/2)]
  	} else {
    		median = (a[n/2 - 1] + a[n/2]) / 2
  	}

  	# kiírás: $1 $2 medián eredeti oszlopok
  	printf "%s %s %.6f", $1, $2, median
  	for (i = 0; i < n; i++) {
    		printf " %s", orig[i]
  	}
  	printf "\n"
}
