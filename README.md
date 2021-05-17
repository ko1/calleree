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
    14  foo
    15
    16  pp Calleree.result
```

And this program shows:

```
[[["test.rb", 14], ["test.rb", 5], 1],
 [["test.rb", 16], ["/mnt/c/ko1/src/rb/calleree/lib/calleree.rb", 24], 1]]
```

The `Calleree.result` method returns an array of arrays which contains `[[caller_path, caller_line], [callee_path, callee_line], called_caount]`.

In this case, `foo` at `["test.rb", 5]` is called at `["test.rb", 14]` only once.

* You can stop the analisys with `Calleree.stop`. `Calleree.start` will continue the analysis.
* You can use block with `Calleree.start do ... end`.
* You can clear the result if `Calleree.result(clear: true)` is passed.

## Development

After checking out the repo, run `bin/setup` to install dependencies. Then, run `rake test` to run the tests. You can also run `bin/console` for an interactive prompt that will allow you to experiment.

To install this gem onto your local machine, run `bundle exec rake install`. To release a new version, update the version number in `version.rb`, and then run `bundle exec rake release`, which will create a git tag for the version, push git commits and tags, and push the `.gem` file to [rubygems.org](https://rubygems.org).

## Contributing

Bug reports and pull requests are welcome on GitHub at https://github.com/ko1/calleree.

## License

The gem is available as open source under the terms of the [MIT License](https://opensource.org/licenses/MIT).
