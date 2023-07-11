#!/usr/bin/perl

print "Content-type: text/html\r\n\r\n";

my $query_string = $ENV{'QUERY_STRING'};

my @params = split(/&/, $query_string);

my %form_data;
foreach my $param (@params) {
    my ($key, $value) = split(/=/, $param);
    $form_data{$key} = $value;
}

my $fname = $form_data{'fname'};
my $lname = $form_data{'lname'};

print "<html>\r\n";
print "<body>\r\n";
print "<p>Your name is $fname $lname!</p>\r\n"; 
print "</body>\r\n";
print "</html>\r\n";
