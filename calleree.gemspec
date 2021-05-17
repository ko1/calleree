require_relative 'lib/calleree/version'

Gem::Specification.new do |spec|
  spec.name          = "calleree"
  spec.version       = Calleree::VERSION
  spec.authors       = ["Koichi Sasada"]
  spec.email         = ["ko1@atdot.net"]

  spec.summary       = %q{Calleree helps to analyze Ruby's caller-callee relationships.}
  spec.description   = %q{Calleree helps to analyze Ruby's caller-callee relationships.}
  spec.homepage      = "https://github.com/ko1/calleree"
  spec.license       = "MIT"
  spec.required_ruby_version = Gem::Requirement.new(">= 2.3.0")

  spec.metadata["homepage_uri"] = spec.homepage
  spec.metadata["source_code_uri"] = "https://github.com/ko1/calleree"

  # Specify which files should be added to the gem when it is released.
  # The `git ls-files -z` loads the files in the RubyGem that have been added into git.
  spec.files         = Dir.chdir(File.expand_path('..', __FILE__)) do
    `git ls-files -z`.split("\x0").reject { |f| f.match(%r{^(test|spec|features)/}) }
  end
  spec.bindir        = "exe"
  spec.executables   = spec.files.grep(%r{^exe/}) { |f| File.basename(f) }
  spec.require_paths = ["lib"]
  spec.extensions    = ["ext/calleree/extconf.rb"]
end
