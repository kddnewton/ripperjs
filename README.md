# ripperjs

A wrapper for the ripper Ruby library.

## Getting started

Ensure you have the following prerequisites installed:

* `gcc`
* `ruby-devel`
* `node`
* `yarn`

Run `yarn` in the root of the repository to get the JavaScript dependencies. Then run `yarn build` to build the necessary library.

You can now run `bin/ripper "..."` to parse ruby code into s-expressions, where the `...` is valid ruby code.

## Tests

Run `yarn test` to run the tests.
