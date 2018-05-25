const path = require("path");
const { setup, sexp } = require("../build/Release/ripperjs");

setup(path.resolve(__dirname, "ripperjs.rb"));
module.exports = { sexp };
