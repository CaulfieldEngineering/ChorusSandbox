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
  
  // Screenshot path includes version number with cache-busting
  const screenshotPath = `${process.env.PUBLIC_URL}/screenshots/screenshot_${version}.png?v=${Date.now()}`;
  
  return (
    <header className={styles.container}>
      <h1 className={styles.title}>{pluginName}</h1>
      <p className={styles.subtitle}>by {companyName}</p>
      
      <div className={styles.screenshotWrapper}>
        {imageError ? (
          <div className={styles.placeholder}>
            <span className={styles.placeholderText}>Screenshot v{version}</span>
          </div>
        ) : (
          <div className={styles.imageContainer}>
            <img 
              src={screenshotPath}
              alt={`${pluginName} v${version} user interface`}
              className={styles.screenshot}
              onError={() => setImageError(true)}
            />
            {/* Reflection */}
            <img 
              src={screenshotPath}
              alt=""
              className={styles.reflection}
              aria-hidden="true"
              onError={(e) => e.target.style.display = 'none'}
            />
          </div>
        )}
      </div>
    </header>
  );
}

export default Header;
