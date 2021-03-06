#!/usr/bin/env ruby

require '../ext/gmp'

multiplicands = ARGV
random_state = GMP::RandState.new

if multiplicands.size > 1
  m, n = multiplicands[0].to_i, multiplicands[1].to_i
  x = random_state.urandomb(m)
  y = random_state.urandomb(n)
else
  m = multiplicands[0].to_i
  x = random_state.urandomb(m)
  y = x
end

z = GMP::Z()
#t = GMP::time { z = x * y }
t = GMP::time { GMP::Z.mul(z,x,y) }
iterations = (1 + (1e4 / t)).to_i

if multiplicands.size > 1
  print "Multiplying %i-bit number with %i-bit number %i times..." % [m, n, iterations]
else
  print "Squaring a %i-bit number %i times..." % [m, iterations]
end
STDOUT.flush

t0 = GMP::cputime
iterations.times do
  #z = x * y
  GMP::Z.mul(z,x,y)
end
ti = GMP::cputime - t0
  
puts "done!"
ops_per_sec = 1000.0 * iterations / ti
f = 100.0
decimals = 0
while true
  decimals += 1
  break if ops_per_sec > f
  f = f * 0.1
end

puts "RESULT: %#{decimals}f operations per second\n" % ops_per_sec
