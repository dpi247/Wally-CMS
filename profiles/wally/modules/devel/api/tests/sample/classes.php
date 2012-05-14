<?php

/**
 * @file Object-oriented tests.
 */

/**
 * @defgroup class_samples Class Samples
 *
 * A sample group of classes. Should not include members.
 *
 * @{
 */

/**
 * Sample class.
 */
class Sample extends ClassNotDefinedHere implements SampleInterface {
  /**
   * A class constant.
   */
  const constant = 'constant value';

  /**
   * A property.
   *
   * @var SampleInterface
   */
  private $property = 'variable value';

  /**
   * Metasyntatic member function.
   *
   * @throws SampleException when it all goes wrong.
   */
  public function foo() {
    // Test linking to a method.
    $x = self::baz();

    // Test linking to a property.
    $this->property = 3;

    // Test linking to a constant.
    $y = $this->constant;

    // But this shouldn't be a link, wrong syntax.
    bar();
  }

  /**
   * Only implemented in children.
   */
  public function baz() {
    // This should be a link.
    Sample::foo();

    // This should link to a search.
    $x->bar();
  }
}

/**
 * Sample interface.
 */
interface SampleInterface {
  /**
   * Implement this API.
   */
  public function foo();
}

/**
 * Subclass.
 *
 * @see Sample::foo() should be a link
 */
class SubSample extends Sample implements SampleInterfaceTwo, InterfaceNotDefinedHere {
  // Not documented (this is intentional for testing).
  public function bar() {
    // This should link to parent method.
    $x = parent::foo();

    // This should link to the parent method, which is not overridden on
    // this class.
    $this->baz();
  }
}

/**
 * Another Sample interface.
 */
interface SampleInterfaceTwo {
  /**
   * A public method.
   */
  public function bar();
}

$random_assignment_not_to_be_parsed = NULL;

abstract class Sample2 implements SampleInterface {
  public function baz() {
  }
}

/**
 * @} end samples
 */
