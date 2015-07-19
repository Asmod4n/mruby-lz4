# mruby-lz4

Example
=======

```ruby
text = "hello hello world world"

compressed = LZ4.compress text

LZ4.decompress(compressed, text.bytesize) == text

```
