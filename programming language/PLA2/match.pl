#!/usr/bin/perl
open FILE, "input.txt" or die $!;
while(<FILE>){
	chomp($_);
	if($_ =~ /hi/){
		print "$_ contains hi\n";
	}
	my $count1 = () = $_ =~ /(a|e|i|o|u)/g;
	if($count1 == 1){
		print "$_ contains one vowel characters\n";
	}
	my $count2 = () = $_ =~ /l/g;
	if($count2 >= 2){
		print "$_ contains two or more l\n";
	}
	if($_ =~ /^h/ && $_ =~ /t$/){
		print "$_ begins with h and end with t\n"
	}
}
