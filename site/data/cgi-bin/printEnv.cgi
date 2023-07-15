#!/usr/bin/env perl

=head1 DESCRIPTION

printenv — a CGI program that just prints its environment

=cut
use Cwd;
my $path = getcwd();

print "Content-Type: text/plain\n\n";

print "Script executed from path: $path\n";
foreach ( sort keys %ENV ) {
    print "$_=\"$ENV{$_}\"\n";
}
