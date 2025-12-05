// ============================================================================
// DOWNLOADS COMPONENT
// Displays download links for Mac and Windows installers
// ============================================================================

import React from 'react';
import config from '../../config';
import styles from './Downloads.module.css';

/**
 * Downloads Component
 * Renders download buttons for the latest version installers
 */
function Downloads() {
  const { version, pluginName } = config;
  const macUrl = config.getDownloadUrl('mac');
  const winUrl = config.getDownloadUrl('windows');

  return (
    <section className={styles.container}>
      <h2 className={styles.title}>Download {pluginName}</h2>
      <p className={styles.version}>Version {version}</p>
      
      <div className={styles.buttons}>
        <a 
          href={macUrl} 
          className={`${styles.button} ${styles.mac}`}
          download
        >
          <span className={styles.icon}>&#63743;</span>
          <span className={styles.label}>
            <span className={styles.platform}>macOS</span>
            <span className={styles.format}>.pkg installer</span>
          </span>
        </a>
        
        <a 
          href={winUrl} 
          className={`${styles.button} ${styles.windows}`}
          download
        >
          <span className={styles.icon}>&#8862;</span>
          <span className={styles.label}>
            <span className={styles.platform}>Windows</span>
            <span className={styles.format}>.exe installer</span>
          </span>
        </a>
      </div>
    </section>
  );
}

export default Downloads;

