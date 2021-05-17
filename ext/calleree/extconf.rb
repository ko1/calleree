require "mkmf"
require 'rbconfig/sizeof'

unless RbConfig::LIMITS['SIZE_MAX'] == 2**64 - 1
  raise "This library is only work on 64bit CPU."
end

create_makefile("calleree/calleree")
