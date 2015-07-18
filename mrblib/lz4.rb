module LZ4
  class << self
    alias :compress :compress_default
    alias :decompress :decompress_safe
  end
end
