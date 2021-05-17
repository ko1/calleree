[![Ruby](https://github.com/ko1/calleree/actions/workflows/ruby.yml/badge.svg)](https://github.com/ko1/calleree/actions/workflows/ruby.yml)

# Calleree

Calleree helps to analyze Ruby's caller-callee relationships.
Note that this tool consumes memory and introduces additional overhead because of dynamic analysis.

## Installation

Add this line to your application's Gemfile:

```ruby
gem 'calleree'
```

And then execute:

    $ bundle install

Or install it yourself as:

    $ gem install calleree

## Usage

```ruby
     1
     2  require 'calleree'
     3
     4  def foo
     5    :foo
     6  end
     7
     8  def bar
     9    foo
    10  end
    11
    12  Calleree.start
    13
    14  bar
    15
    16  pp Calleree.result
```

And this program shows:

```
[[["test.rb", 14], ["test.rb", 9], 1],
 [["test.rb", 9], ["test.rb", 5], 1],
 [["test.rb", 16], ["/mnt/c/ko1/src/rb/calleree/lib/calleree.rb", 23], 1]]
```

The `Calleree.result` method returns an array of arrays which contains `[[caller_path, caller_line], [callee_path, callee_line], called_caount]`.

In this case:

* A method `bar` at `["test.rb", 9]` is called from `["test.rb", 14]` only once.
* A method `foo` at `["test.rb", 5]` is called from `bar` at  `["test.rb", 9]` once.
* A method `Calleree.result` at `["...calleree.rb", 23]` is called from `["test.rb", 16]`.

Additional usage:

* You can use block with `Calleree.start do ... end`.
* You can stop the analisys with `Calleree.stop`. `Calleree.start` will continue the analysis.
* You can clear the result if `Calleree.result(clear: true)` is passed.

## Performance

The feature of this library can be implemented by Ruby (see `manual` method in the following code).
The difference is the performance.

```ruby
require 'calleree'

def foo
  :foo
end

def bar
  foo
end

def demo
  100_000.times{
    bar
  }
end

def manual
  result = Hash.new(0)
  TracePoint.new(:call){
    callee, caller = caller_locations(1, 2).map{|loc| [loc.path, loc.lineno]}
    result[[caller, callee]] += 1
  }.enable{
    yield
  }
  result
end

require 'benchmark'

Benchmark.bm(10){|x|
  x.report('none'){ demo }
  x.report('manual'){ manual{ demo } }
  x.report('calleree'){ Calleree.start{ demo } }
}
```

The result of above benchmark is:

```
                 user     system      total        real
none         0.004860   0.000972   0.005832 (  0.005828)
manual       0.864746   0.000000   0.864746 (  0.864833) # about x175
calleree     0.031616   0.000000   0.031616 (  0.031620) # about x6
```

## Development

After checking out the repo, run `bin/setup` to install dependencies. Then, run `rake test` to run the tests. You can also run `bin/console` for an interactive prompt that will allow you to experiment.

To install this gem onto your local machine, run `bundle exec rake install`. To release a new version, update the version number in `version.rb`, and then run `bundle exec rake release`, which will create a git tag for the version, push git commits and tags, and push the `.gem` file to [rubygems.org](https://rubygems.org).

## Contributing

Bug reports and pull requests are welcome on GitHub at https://github.com/ko1/calleree.

## License

The gem is available as open source under the terms of the [MIT License](https://opensource.org/licenses/MIT).
