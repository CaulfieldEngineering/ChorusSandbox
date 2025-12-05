// ============================================================================
// FEATURES COMPONENT
// Plugin feature list
// ============================================================================

import React from 'react';
import styles from './Features.module.css';

/**
 * Feature list - edit this array to update features
 */
const FEATURES = [
  {
    title: 'Multi-Voice Engine',
    description: 'Up to 10 independent chorus voices with individual attenuation control'
  },
  {
    title: 'Stereo Processing',
    description: 'Mono, Stereo, and Mid-Side modes with adjustable stereo spread'
  },
  {
    title: 'Built-in Filtering',
    description: 'High-pass and low-pass filters to shape the wet signal'
  },
  {
    title: 'Precise Control',
    description: 'Fine-tune rate, depth, and base delay for the perfect chorus sound'
  }
];

/**
 * Features Component
 * Displays plugin features in a clean grid
 */
function Features() {
  return (
    <section id="features" className={styles.container}>
      <h2 className={styles.title}>Features</h2>
      
      <div className={styles.grid}>
        {FEATURES.map((feature, index) => (
          <div key={index} className={styles.feature}>
            <h3 className={styles.featureTitle}>{feature.title}</h3>
            <p className={styles.featureDescription}>{feature.description}</p>
          </div>
        ))}
      </div>
    </section>
  );
}

export default Features;

