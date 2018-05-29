const { sexp } = require("../src");

test("converts to s-expressions", () => {
  const { type } = sexp("a = 1; b = 2; a + b");
  expect(type).toEqual("program");
});

test("throws an error when parsing invalid Ruby code", () => {
  expect(() => sexp("<>")).toThrow("Invalid");
});

test("attachs comments", () => {
  const result = sexp("a\n# comment\nb");
  expect(result.body[0].body[0].comment).not.toBe(null);
});
