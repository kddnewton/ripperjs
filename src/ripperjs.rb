require 'ripper'

class RipperJS < Ripper::SexpBuilder
  attr_reader :comments

  def initialize(*args)
    super
    @comments = []
  end

  def parse
    super.tap do |result|
      return if error?
    end
  end

  def self.sexp(src, filename = '-', lineno = 1)
    new(src, filename, lineno).parse
  end

  private

  SCANNER_EVENTS.each do |event|
    module_eval(<<-End, __FILE__, __LINE__ + 1)
      def on_#{event}(tok)
        { type: :@#{event}, body: tok, lineno: lineno, column: column, comment: nil }
      end
    End
  end

  events = private_instance_methods(false).grep(/\Aon_/) { $'.to_sym }
  (PARSER_EVENTS - events).each do |event|
    module_eval(<<-End, __FILE__, __LINE__ + 1)
      def on_#{event}(*args)
        { type: :#{event}, body: args, lineno: lineno, column: column, comment: nil }
      end
    End
  end

  def on_comment(comment)
    @comments << { type: :comment, body: comment, lineno: lineno, column: column }
  end

  def on_embdoc_beg(comment)
    @comments << { type: :embdoc, body: comment, lineno: lineno, column: column }
  end

  def on_embdoc(comment)
    @comments.last[:body] << comment
  end

  def on_embdoc_end(comment)
    @comments.last[:body] << comment
  end
end
