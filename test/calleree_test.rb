require "test_helper"

class CallereeTest < Minitest::Test
  def test_that_it_has_a_version_number
    refute_nil ::Calleree::VERSION
  end

  def foo
    :foo
  end

  def bar
    foo
  end

  def test_result
    r = Calleree.start do
      bar
    end

    assert_equal :foo, r
    2.times do
      result = Calleree.result
      assert_equal 2, result.size

      # file
      assert_equal __FILE__, result[0][0][0]
      assert_equal __FILE__, result[0][1][0]
      assert_equal __FILE__, result[1][0][0]
      assert_equal __FILE__, result[1][1][0]

      # cnt
      assert_equal 1, result[0][2]
      assert_equal 1, result[1][2]
    end

    Calleree.result(clear: true)
    assert_equal [], Calleree.result
  end
end
