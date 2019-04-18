use File::Slurp;

my $f = "sample.txt";

my $s = read_file($f);

print length($s);
#print substr($s, 1859, 2);