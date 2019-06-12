#!/bin/awk -f
BEGIN {
	ip = 0;
	FS = ".";
}

{
	ip = 0;
	if ($0 ~ /#.*/)
		next;
	if ($0 !~ /[0-9]+\.[0-9]+\.[0-9]+\.[0-9]+/) {
		print "Wrong IP in LINE: "NR;
		exit;
	}
	if (NF != 4 ||
		(length($1) < 1 || length($1) > 3 || $1 > 255) ||
		(length($2) < 1 || length($2) > 3 || $2 > 255) ||
		(length($3) < 1 || length($3) > 3 || $3 > 255) ||
		(length($4) < 1 || length($4) > 3 || $4 > 255)) {

		print "Wrong IP in LINE: "NR;
		exit;
	}

	ip = lshift($1, 24) + lshift($2, 16) + lshift($3, 8) + $4;
	print ip;
}
