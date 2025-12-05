// ============================================================================
// HELLO WORLD COMPONENT
// ============================================================================

import React from 'react';
import styles from './HelloWorld.module.css';

/**
 * HelloWorld Component
 * Displays a greeting message with styled typography
 */
function HelloWorld() {
  return (
    <section className={styles.container}>
      <h1 className={styles.greeting}>Hello!</h1>
    </section>
  );
}

export default HelloWorld;

