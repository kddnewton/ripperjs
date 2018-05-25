const { sexp } = require("../build/Release/ripperjs");

test("converts to s-expressions", () => {
  const { type } = sexp("a = 1; b = 2; a + b");
  expect(type).toEqual("program");
});

test("throws an error when parsing invalid Ruby code", () => {
  expect(() => sexp("<>")).toThrow("Invalid");
});
