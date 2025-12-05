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
  const { pluginName, companyName, version } = config;
  const [imageError, setImageError] = useState(false);
  
  // Screenshot path includes version number: screenshot_x.x.x.png
  const screenshotPath = `${process.env.PUBLIC_URL}/images/screenshot_${version}.png`;
  
  return (
    <header className={styles.container}>
      <h1 className={styles.title}>{pluginName}</h1>
      <p className={styles.subtitle}>by {companyName}</p>
      
      {!imageError && (
        <div className={styles.screenshotWrapper}>
          <img 
            src={screenshotPath}
            alt={`${pluginName} v${version} user interface`}
            className={styles.screenshot}
            onError={() => setImageError(true)}
          />
        </div>
      )}
    </header>
  );
}

export default Header;
