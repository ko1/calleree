require "calleree/version"
require "calleree/calleree"

module Calleree
  def self.start
    if block_given?
      begin
        Calleree._start
        yield
      ensure
        Calleree._stop
      end
    else
      Calleree._start
    end
  end

  def self.stop
    Calleree._stop
  end

  def self.result clear: false
    posnum_set = []
    Calleree.raw_posmap.each{|file, lines|
      lines.each_with_index{|posnum, line|
        posnum_set[posnum] = [file, line] if posnum
      }
    }
    # pp posnum_set
    rs = []
    Calleree.raw_result(clear).each{|i, j, cnt|
      er = posnum_set[i]
      ee = posnum_set[j]
      rs << [er, ee, cnt] if er && ee
    }
    rs
  end
end
