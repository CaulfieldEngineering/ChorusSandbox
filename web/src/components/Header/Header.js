// ============================================================================
// HEADER COMPONENT
// Displays the plugin name, company branding, and screenshot with lightbox
// ============================================================================

import React, { useState, useEffect } from 'react';
import config from '../../config';
import styles from './Header.module.css';

/**
 * Header Component
 * Displays the plugin name, company attribution, and plugin screenshot
 * Click on screenshot to open full-size lightbox
 */
function Header() {
  const { pluginName, companyName, version } = config;
  const [imageError, setImageError] = useState(false);
  const [lightboxOpen, setLightboxOpen] = useState(false);
  
  // Screenshot path includes version number with cache-busting
  const screenshotPath = `${process.env.PUBLIC_URL}/screenshots/screenshot_${version}.png?v=${Date.now()}`;
  
  // Close lightbox on Escape key
  useEffect(() => {
    const handleEscape = (e) => {
      if (e.key === 'Escape') setLightboxOpen(false);
    };
    if (lightboxOpen) {
      document.addEventListener('keydown', handleEscape);
      document.body.style.overflow = 'hidden';
    }
    return () => {
      document.removeEventListener('keydown', handleEscape);
      document.body.style.overflow = '';
    };
  }, [lightboxOpen]);
  
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
              alt={`${pluginName} v${version} user interface - click to enlarge`}
              className={styles.screenshot}
              onError={() => setImageError(true)}
              onClick={() => setLightboxOpen(true)}
              title="Click to view full size"
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
      
      {/* Lightbox modal */}
      {lightboxOpen && (
        <div 
          className={styles.lightbox} 
          onClick={() => setLightboxOpen(false)}
        >
          <span className={styles.lightboxClose}>&times;</span>
          <img 
            src={screenshotPath}
            alt={`${pluginName} v${version} user interface`}
            className={styles.lightboxImage}
            onClick={(e) => e.stopPropagation()}
          />
        </div>
      )}
    </header>
  );
}

export default Header;
