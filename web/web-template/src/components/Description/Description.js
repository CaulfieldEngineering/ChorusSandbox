// ============================================================================
// DESCRIPTION COMPONENT
// Plugin description and overview
// ============================================================================

import React from 'react';
import styles from './Description.module.css';

/**
 * Description Component
 * Displays the plugin overview and description
 */
function Description() {
  return (
    <section id="description" className={styles.container}>
      <p className={styles.text}>
        A lush, multi-voice chorus effect designed for modern music production. 
        ChorusSandbox delivers rich, animated textures with precise stereo control 
        and built-in filtering for seamless integration into any mix.
      </p>
    </section>
  );
}

export default Description;

