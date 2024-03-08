#!/usr/bin/perl

#  Copyright (C) 2002-2024 Michael S. D'Errico.  All Rights Reserved.
#
#  This source code is the property of Michael S. D'Errico and is
#  protected under international copyright laws.
#  
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of version 3 of the GNU General Public License as 
#  published by the Free Software Foundation.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
#  A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
#  HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
#  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
#  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
#  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
#  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
#  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <https://www.gnu.org/licenses/>.
#
#  Options for Contacting the Author:
#
#    email name:   mikestoolbox
#    email domain: pobox.com
#    X/Twitter:    @mikestoolbox
#    mail:         Michael D'Errico
#                  10161 Park Run Drive, Suite 150
#                  Las Vegas, NV 89145


$counts[0] = 150;
$counts[1] = 125;
$counts[2] = 100;
$counts[3] =  75;
$counts[4] =  50;
$counts[5] =  25;

sub by_class
{
    if ($a =~ /(\S+)::(\~?\w+)/o)
    {
        local $class_a  = $1;
        local $method_a = $2;

        if ($b =~ /(\S+)::(\~?\w+)/o)
        {
            local $class_b  = $1;
            local $method_b = $2;

            if ($class_a eq $class_b)
            {
                return $method_a cmp $method_b;
            }
            else
            {
                return $class_a cmp $class_b;
            }
        }
        else
        {
            return -1;
        }
    }
    elsif ($b =~ /(\S+)::(\~?\w+)/o)
    {
        return 1;
    }
    else
    {
        return $a cmp $b;
    }
}

opendir DIR, "../include/mikestoolbox";
push @files, readdir DIR;
closedir DIR;

grep s:^:../include/mikestoolbox/:o, @files;

opendir DIR, ".";
push @files, readdir DIR;
closedir DIR;

opendir DIR, "UNIX";
push @unix, readdir DIR;
closedir DIR;

opendir DIR, "WIN32";
push @win, readdir DIR;
closedir DIR;

grep s:^:UNIX/:o, @unix;
grep s:^:WIN32/:o, @win;
push @files, @unix;
# push @files, @win;

while ($file = shift @files)
{
    next unless $file =~ /\.(inl|cpp)$/o;

    open FILE, "<$file";

    @lines = <FILE>;

    close FILE;

    $function  = "";
    $start     = "";
    $num_lines = 0;

    while ($line = shift @lines)
    {
        if ($line =~ /^\}/o)
        {
            next unless $function;
            next if $function =~ /NO_METRIC/o;
# next if $function =~ / Tls\S*::/o;

            $significant += $num_lines;

            $under[0] += $num_lines if ($num_lines <= $counts[0]);
            $under[1] += $num_lines if ($num_lines <= $counts[1]);
            $under[2] += $num_lines if ($num_lines <= $counts[2]);
            $under[3] += $num_lines if ($num_lines <= $counts[3]);
            $under[4] += $num_lines if ($num_lines <= $counts[4]);
            $under[5] += $num_lines if ($num_lines <= $counts[5]);

            ++$funcs[0] if ($num_lines > $counts[0]);
            ++$funcs[1] if ($num_lines > $counts[1]);
            ++$funcs[2] if ($num_lines > $counts[2]);
            ++$funcs[3] if ($num_lines > $counts[3]);
            ++$funcs[4] if ($num_lines > $counts[4]);
            ++$funcs[5] if ($num_lines > $counts[5]);

            $max_lines    = $num_lines if ($num_lines > $max_lines);
            $squared     += $num_lines * $num_lines;
            $sum_squared += $squared;

            $num_lines = "$num_lines ";

            $length = length $num_lines;

            $blank = " " x (5 - $length);

            $function =~ s/(.{93}).*/$1/;

            push @output, "$blank$num_lines$function" if $function;

            $num_lines = 0;
            $function  = "";
        }
        elsif ($line =~ /^\{/o)
        {
            $function  = $start;
            $num_lines = 0;
            ++$num_funcs;
        }
        else
        {
            if ($line =~ m:^(|[^ #/].*)$:o)
            {
                $start = $line;
            }

            ++$num_lines if $line !~ m:^ *[{}]? *(//.*)?$:o;
            ++$total_lines;
        }
    }
}

# @output = sort by_class @output;
@output = sort @output;

$mean   = $significant / $num_funcs;
$median = $output[$num_funcs/2];
$median =~ s/^ *(\d+).*\s*/$1/o;
$mean_sq = sqrt ($sum_squared) / $num_funcs;

$under[0] /= $significant;
$under[1] /= $significant;
$under[2] /= $significant;
$under[3] /= $significant;
$under[4] /= $significant;
$under[5] /= $significant;

$metric = ($under[0] * $under[1] * $under[2] * $under[3] * $under[4] * $under[5]) ** (1.0/3.0);
$metric = int ($metric * 1000.0 + 0.5);

$mean     = int (100 * $mean)    / 100.0;
$mean_sq  = int (100 * $mean_sq) / 100.0;

$under[0] = int (100.0 * $under[0] + 0.5);
$under[1] = int (100.0 * $under[1] + 0.5);
$under[2] = int (100.0 * $under[2] + 0.5);
$under[3] = int (100.0 * $under[3] + 0.5);
$under[4] = int (100.0 * $under[4] + 0.5);
$under[5] = int (100.0 * $under[5] + 0.5);

print @output;

$counts[0] = " $counts[0]" if (length $counts[0] < 3);
$counts[1] = " $counts[1]" if (length $counts[1] < 3);
$counts[2] = " $counts[2]" if (length $counts[2] < 3);
$counts[3] = " $counts[3]" if (length $counts[3] < 3);
$counts[4] = " $counts[4]" if (length $counts[4] < 3);
$counts[5] = " $counts[5]" if (length $counts[5] < 3);

$funcs[0] = $funcs[0] ? "   ($funcs[0])" : "";
$funcs[1] = $funcs[1] ? "   ($funcs[1])" : "";
$funcs[2] = $funcs[2] ? "   ($funcs[2])" : "";
$funcs[3] = $funcs[3] ? "   ($funcs[3])" : "";
$funcs[4] = $funcs[4] ? "   ($funcs[4])" : "";
$funcs[5] = $funcs[5] ? "   ($funcs[5])" : "";

print "\n";
print "functions:         $num_funcs\n";
print "total lines:       $significant ($total_lines)\n";
print "average lines:     $mean\n";
print "power measure:     $mean_sq\n";
print "median lines:      $median\n";
print "under $counts[0] lines:   $under[0] %$funcs[0]\n";
print "under $counts[1] lines:   $under[1] %$funcs[1]\n";
print "under $counts[2] lines:   $under[2] %$funcs[2]\n";
print "under $counts[3] lines:   $under[3] %$funcs[3]\n";
print "under $counts[4] lines:   $under[4] %$funcs[4]\n";
print "under $counts[5] lines:   $under[5] %$funcs[5]\n";
print "\n";
print "metric:            $metric\n";
print "\n";

