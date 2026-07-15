#include "railos_github_packages.hxx"

size_t RailOSPkg::write_to_string_(void *contents, size_t size, size_t nmemb,
                                   std::string *output) {
  size_t totalSize = size * nmemb;
  output->append(static_cast<char *>(contents), totalSize);
  return totalSize;
}

std::vector<std::string> RailOSPkg::get_repositories(QWidget *parent) {
  std::vector<std::string> lines_;
  CURL *curl_ = curl_easy_init();
  CURLcode res_;
  curl_easy_setopt(curl_, CURLOPT_URL, GITHUB_RAILOS_PKG_REGISTRY_.c_str());

  std::string buffer_;
  struct curl_slist *headers_ = nullptr;
  headers_ =
      curl_slist_append(headers_, "User-Agent: C++ libcurl Repo Fetcher");
  curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers_);
  curl_easy_setopt(curl_, CURLOPT_FAILONERROR, true);
  curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYPEER, false);
  curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &buffer_);
  curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, write_to_string_);
  curl_easy_setopt(curl_, CURLOPT_FOLLOWLOCATION, 1);
  curl_easy_setopt(curl_, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2);
  res_ = curl_easy_perform(curl_);

  long http_code_ = 0;
  if (res_ == CURLE_OK) {
    curl_easy_getinfo(curl_, CURLINFO_RESPONSE_CODE, &http_code_);
  }

  curl_slist_free_all(headers_);
  curl_easy_cleanup(curl_);

  if (res_ != CURLE_OK) {
    const QString alert_ = "Failed to retrieve project listing from URL:\n" +
                           QString::fromStdString(GITHUB_RAILOS_PKG_REGISTRY_) +
                           "\nwith error code " + QString::number(res_) +
                           ".\n" +
                           "Repositories can still be manually entered.";
    QMessageBox::warning(parent, QMessageBox::tr("Registry Query Failure"),
                         QMessageBox::tr(alert_.toStdString().c_str()));
    return lines_;
  }

  std::stringstream ss(buffer_);
  std::string line_;
  while (std::getline(ss, line_)) {
    // Clean up potential Carriage Return (\r) windows line endings if they
    // exist
    if (!line_.empty() && line_.back() == '\r') {
      line_.pop_back();
    }

    // Skip empty lines
    if (!line_.empty()) {
      lines_.push_back(line_);
    }
  }

  return lines_;
}