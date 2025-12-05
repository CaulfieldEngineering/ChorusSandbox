// ============================================================================
// HEADER COMPONENT
// Displays the plugin name, company branding, and screenshot
// ============================================================================

import React, { useState } from 'react';
import config from '../../config';
import styles from './Header.module.css';

/**
 * Header Component
 * Displays the plugin name, company attribution, and plugin screenshot
 */
function Header() {
  const { pluginName, companyName } = config;
  const [imageError, setImageError] = useState(false);
  
  // Screenshot path relative to public folder
  const screenshotPath = `${process.env.PUBLIC_URL}/images/screenshot.png`;
  
  return (
    <header className={styles.container}>
      <h1 className={styles.title}>{pluginName}</h1>
      <p className={styles.subtitle}>by {companyName}</p>
      
      {!imageError && (
        <div className={styles.screenshotWrapper}>
          <img 
            src={screenshotPath}
            alt={`${pluginName} user interface`}
            className={styles.screenshot}
            onError={() => setImageError(true)}
          />
        </div>
      )}
    </header>
  );
}

export default Header;
