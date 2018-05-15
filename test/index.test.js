const ripperjs = require("../src");

test("converts to s-expressions", () => {
  const result = ripperjs.sexp("a = 1; b = 2; a + b");
});
